/*
 * mcac
 * MCA Control
 *   プロトタイプ
 *   参考 usb-skeleton.c
 *
 * Memory card type
 *   Empty  '0'
 *   PS1    '1'
 *   PS2    '2'
 *
 * TODO 不要なコードもまだあると思う
 * TODO その他コード整理
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <linux/uaccess.h>
#include <linux/usb.h>
#include <linux/mutex.h>

#include "mc.h"
#include "mca.h"

#define DRIVER_NAME "mcac"
#define DRIVER_DESC "device driver for MCA(check memory card type)"


/* table of devices that work with this driver */
static const struct usb_device_id mca_table[] = {
	{ USB_DEVICE(MCA_VEND_ID, MCA_PROD_ID) },
	{ } /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, mca_table);


/* Get a minor range for your devices from the usb maintainer */
#define MINOR_COUNT 1

/*
 * kref:  linux kernelが提供する参照カウンタ
 * io_mutex:  USB転送処理中にクローズされるのを防止
 */
/* Structure to hold all of our device specific stuff */
struct usb_mca {
	struct usb_device *udev;  /* the usb device for this device */
	struct usb_interface *interface;  /* the interface for this device */
	struct kref kref;
	struct mutex io_mutex;  /* synchronize I/O with disconnect */

	char mca_sbuf[2];  /* MCA送信用バッファ */
	char mca_rbuf[2];  /* MCA受信用バッファ */
};

static struct usb_driver mca_driver;

static void mca_delete(struct kref *kref)
{
	struct usb_mca *dev = container_of(kref, struct usb_mca, kref);

	usb_put_dev(dev->udev);
	kfree(dev);
}

static int mca_open(struct inode *inode, struct file *file)
{
	struct usb_mca *dev;
	struct usb_interface *interface;
	int subminor;
	int retval = 0;

	nonseekable_open(inode, file);
	subminor = iminor(inode);

	interface = usb_find_interface(&mca_driver, subminor);
	if (!interface) {
		pr_err("%s - error, can't find device for minor %d\n",
			__func__, subminor);
		retval = -ENODEV;
		goto exit;
	}

	dev = usb_get_intfdata(interface);
	if (!dev) {
		retval = -ENODEV;
		goto exit;
	}

	/* increment our usage count for the device */
	kref_get(&dev->kref);

	/* save our object in the file's private structure */
	file->private_data = dev;

exit:
	return retval;
}

static int mca_release(struct inode *inode, struct file *file)
{
	struct usb_mca *dev;

	dev = file->private_data;
	if (!dev)
		return -ENODEV;

	/* decrement the count on our device */
	kref_put(&dev->kref, mca_delete);
	return 0;
}

/*
 * MCAに刺さっているメモリーカードの種類を確認する
 */
static ssize_t read_mc_type(struct usb_mca *dev)
{
	int retval = 0;
	int count = 0;

	/* 確認用の命令 */
	dev->mca_sbuf[0] = 0xaa;
	dev->mca_sbuf[1] = 0x40;

	/* 命令を送信 */
	retval = usb_bulk_msg(dev->udev,
			usb_sndbulkpipe(dev->udev, MCA_EP_OUTPUT),
			dev->mca_sbuf,
			MCA_LEN_CHECK_MC_BUF,
			&count,
			HZ * MCA_TIMEOUT);
	if (retval < 0)
		return retval;

	/* 結果を受信 */
	retval = usb_bulk_msg(dev->udev,
			usb_rcvbulkpipe(dev->udev, MCA_EP_INPUT),
			dev->mca_rbuf,
			MCA_LEN_CHECK_MC_BUF,
			&count,
			HZ * MCA_TIMEOUT);
	if (retval < 0)
		return retval;

	return count;
}

static ssize_t mca_read(struct file *file, char *buffer, size_t count,
			 loff_t *ppos)
{
	struct usb_mca *dev = NULL;
	int rv = 0;

	dev = file->private_data;

	/* no concurrent readers */
	rv = mutex_lock_interruptible(&dev->io_mutex);
	if (rv < 0)
		return rv;

	/* 既にUSBと通信していた場合、0を返す */
	if (*ppos > 0)
		goto exit;

	if (!dev->interface) {		/* disconnect() was called */
		rv = -ENODEV;
		goto exit;
	}

	/* USB通信処理 */
	rv = read_mc_type(dev);
	if (rv < 0)
		goto exit;
	if ((rv < MCA_LEN_CHECK_MC_BUF) ||
			(dev->mca_rbuf[0] != MCA_RCV_HEAD)) {
		rv = -EIO;
		goto exit;
	}

	dev->mca_rbuf[MCA_RCV_CHECK_POS] += '0';

	/* ユーザ空間へコピー */
	if (copy_to_user(buffer, dev->mca_rbuf + MCA_RCV_CHECK_POS, rv)) {
		rv = -EFAULT;
		goto exit;
	}

	*ppos += rv;

exit:
	mutex_unlock(&dev->io_mutex);

	return rv;
}

static const struct file_operations mca_fops = {
	.owner = THIS_MODULE,
	.read = mca_read,
	.write = NULL,
	.open = mca_open,
	.release = mca_release,
	.llseek = no_llseek,
};

/*
 * デバイスファイルのパーミッションを0666にする
 */
static char *handle_devnode(struct device *dev, umode_t *mode)
{
	if (mode)
		*mode = 0666;
	return NULL;
}

/*
 * usb class driver info in order to get a minor number from the usb core,
 * and to have the device registered with the driver core
 */
static struct usb_class_driver mca_class = {
	.name = DRIVER_NAME,
	.devnode = handle_devnode,
	.fops = &mca_fops,
	.minor_base = MINOR_COUNT,
};

static int mca_probe(struct usb_interface *interface,
		      const struct usb_device_id *id)
{
	struct usb_mca *dev;
	int retval = -ENOMEM;

	/* allocate memory for our device state and initialize it */
	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		goto error;
	kref_init(&dev->kref);
	mutex_init(&dev->io_mutex);

	dev->udev = usb_get_dev(interface_to_usbdev(interface));
	dev->interface = interface;

	/* save our data pointer in this interface device */
	usb_set_intfdata(interface, dev);

	/* we can register the device now, as it is ready */
	retval = usb_register_dev(interface, &mca_class);
	if (retval) {
		/* something prevented us from registering this driver */
		dev_err(&interface->dev,
			"Not able to get a minor for this device.\n");
		usb_set_intfdata(interface, NULL);
		goto error;
	}

	dev_info(&interface->dev, "%s device now attached", DRIVER_NAME);
	return 0;

error:
	if (dev)
		/* this frees allocated memory */
		kref_put(&dev->kref, mca_delete);
	return retval;
}

static void mca_disconnect(struct usb_interface *interface)
{
	struct usb_mca *dev;

	dev = usb_get_intfdata(interface);
	usb_set_intfdata(interface, NULL);

	/* give back our minor */
	usb_deregister_dev(interface, &mca_class);

	/* prevent more I/O from starting */
	mutex_lock(&dev->io_mutex);
	dev->interface = NULL;
	mutex_unlock(&dev->io_mutex);

	/* decrement our usage count */
	kref_put(&dev->kref, mca_delete);

	dev_info(&interface->dev, "%s now disconnected", DRIVER_NAME);
}

static struct usb_driver mca_driver = {
	.name = DRIVER_NAME,
	.probe = mca_probe,
	.disconnect = mca_disconnect,
	.id_table = mca_table,

	/* mcaはautosuspendは対応していない?ので0 */
	.supports_autosuspend = 0,
};

module_usb_driver(mca_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DRIVER_DESC);
