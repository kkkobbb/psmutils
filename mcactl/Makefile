CC := gcc
CFLAGS := -g -Wall `pkg-config --cflags libusb-1.0`
LDLIBS := `pkg-config --libs libusb-1.0`

DESTDIR :=
prefix := /usr/local
exec_prefix := $(prefix)
bindir := $(exec_prefix)/bin/
srcdir := $(dir $(word 1,$(MAKEFILE_LIST)))

DEST := mcactl
SRC  := main.c operate.c mcaio.c

OBJS := $(patsubst %.c,%.o,$(SRC))


all: $(DEST)

.PHONY: all install TAGS distclean clean cleantags

$(DEST): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LDLIBS)

install: $(DEST)
	mkdir -p $(DESTDIR)$(bindir)
	cp $? $(DESTDIR)$(bindir)

TAGS:
	ctags *.c *.h

distclean: clean cleantags

clean:
	$(RM) $(OBJS) $(DEST)

cleantags:
	$(RM) tags



### ヘッダ依存関係 自動解決 ###
DEPEND_DIR := _depend
DEPENDS := $(patsubst %.c,$(DEPEND_DIR)/%.d,$(SRC))

$(DEPEND_DIR)/%.d: $(srcdir)/%.c
	@install -d $(dir $@)
	$(CC) -MM -MG -MF $@ -MT $(patsubst %.c,%.o,$<) $<

.PHONY: cleandep
cleandep:
	$(RM) -r $(DEPEND_DIR)
distclean: cleandep

ifneq "$(findstring clean, $(MAKECMDGOALS))" "clean"
  -include $(DEPENDS)
endif
