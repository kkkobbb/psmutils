/*
 * PocketStation App
 * 画像表示のみ
 */
#include "utils.h"

int save_data = 0;


void
draw_image()
{
	static unsigned int lcd_title[] = {
		0x1cfc7ee0,
		0x3ffffff8,
		0x3ffffffc,
		0x3ffffffe,
		0x7ffffffe,
		0x7adffffc,
		0x75b7d7fc,
		0x2dabedec,
		0x2b6feab6,
		0x6b7ffab6,
		0x7ffffbfe,
		0x7ffffffe,
		0x7ffd7ffe,
		0x7ffd7ffe,
		0x7ffd7ffe,
		0x7ff81efa,
		0x7fc433f8,
		0x3fc031f8,
		0x7ee038dc,
		0xfee038d6,
		0xfef03cfb,
		0xfe0000f9,
		0xfe0100fd,
		0xff0001fe,
		0xff0383ff,
		0xff8007ff,
		0xffc00fff,
		0xffe00fff,
		0xfff00ffd,
		0xfff80ffd,
		0xffcc03fc,
		0xff8003f8,
	};

	write_screen(lcd_title);
}


int main()
{
	init();

	draw_image();

	while (1)
		while (wait_key())
			;
}
