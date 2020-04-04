#include "hed.h"

extern void sleep();
extern void app_exit();
extern void vsync_wait();


void
lcd_write_(p)
	unsigned int *p;
{
	int i;
	int *start = (int *)0xd000100;
	int *end = start + LCD_SIZE;

	while(start < end)
		*start++ = *p++;
}

int
pad_read()
{
	int pad;

	pad = *(volatile int *)0xa000004;
	pad |= *(volatile char *)0x203;
	*(volatile char *)0x203 = 0;
	
	return (pad);
}

void
pad_waitup()
{
	int now_pad;
	now_pad = pad_read() & PAD_ALL;
	while(now_pad == (pad_read() & PAD_ALL))
		;
}

/*
 * Exit?
 */
void
cont_exit()
{
	int timeout;
	int i;
	int *p;
	int status, ostatus;
	int select;
#define	SELECT_CONTINUE	0
#define	SELECT_EXIT	1
	static int bmp_continue[] = {
		0x00000000, 0x7575dddc, 0x15549544, 0x75549544,
		0x15549544, 0x775495dc, 0x00000000
	};
	static int bmp_exit[] = {
		0x00000000, 0x00755c00, 0x00254400, 0x00249c00,
		0x00254400, 0x00255c00, 0x00000000
	};
	select = SELECT_CONTINUE;
	ostatus = pad_read() & 0x19;

	for (timeout = ONE_SEC * 60; timeout > 0; timeout--) {
		vsync_wait();
		p = (int *)0xd000100;
		for (i = 0; i < 32; i++) {
			if (i >= 8 &&
			    i < 8 + sizeof (bmp_continue) / sizeof (int))
				*p++ = select == SELECT_CONTINUE ?
				    ~bmp_continue[i - 8] : bmp_continue[i - 8];
			else if (i >= 17 &&
			    i < 17 + sizeof (bmp_exit) / sizeof (int))
				*p++ = select == SELECT_EXIT ?
				    ~bmp_exit[i - 17] : bmp_exit[i - 17];
			else 
				*p++ = 0;
		}

		status = pad_read() & 0x19;
		if (ostatus != status)
			ostatus = status;
		else
			continue;

		if (status & (1 << 3))
			select = SELECT_EXIT;
		if (status & (1 << 4))
			select = SELECT_CONTINUE;
		if (status & (1 << 0))
			if (select == SELECT_CONTINUE) {
				break;
			} else {
				while (pad_read() & 0x1f)
					;
				app_exit();
			}
	}
}

/*
 * wait key
 */
int
wait_key()
{
	static int bcount, sleep_count;
	int status;

	vsync_wait();
	status = pad_read() & 0x1f;

	if (status) {
		sleep_count = 0;
	} else {
		if (++sleep_count > ONE_SEC * 30) {
			sleep_count = 0;
			sleep();
		}
	}

	if (status & (1 << 0)) {
		while (status) {
			vsync_wait();
			status = pad_read() & 0x1f;

			if (status & (1 << 0))
				bcount++;
			else
				bcount = 0;
			if (bcount >= ONE_SEC * 3) {
				cont_exit();
				bcount = 0;
				status = 0;
			}
		}
		return 0;
	}
	return 1;
}
