# psmutils debパッケージ作成
#
# debian系用
# $(DEB_DIR)以下に *.debファイルを作成する
#
# fakeroot が必要
DIR_EDIT := ../../psmedit/
DIR_MCA := ../../mcactl/

DIR_WORK := $(shell pwd)

DEB_DIR_BASE := $(DIR_WORK)/deb_base/
DEB_DIR_BASE_EDIT := $(DEB_DIR_BASE)/psmedit/
DEB_DIR_BASE_MCA := $(DEB_DIR_BASE)/mcactl/
DEB_DIR := $(DIR_WORK)/_deb/
DEB_DIR_EDIT := $(DEB_DIR)/psmedit/
DEB_DIR_MCA := $(DEB_DIR)/mcactl/


all: deb_edit deb_mca

deb_edit:
	mkdir -p $(DEB_DIR_EDIT)
	cp -r $(DEB_DIR_BASE_EDIT)/* $(DEB_DIR_EDIT)
	make -C $(DIR_EDIT) install prefix=/usr/ DESTDIR=$(DEB_DIR_EDIT)
	fakeroot dpkg-deb --build $(DEB_DIR_EDIT) $(DEB_DIR)

deb_mca:
	mkdir -p $(DEB_DIR_MCA)
	cp -r $(DEB_DIR_BASE_MCA)/* $(DEB_DIR_MCA)
	make -C $(DIR_MCA) install prefix=/usr/ DESTDIR=$(DEB_DIR_MCA)
	fakeroot dpkg-deb --build $(DEB_DIR_MCA) $(DEB_DIR)

clean:
	$(RM) -r $(DEB_DIR)
