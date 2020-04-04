/*
 * 色々
 */
#include "pks.h"

/*
 * 画面を上書きする
 *
 * scr_buf  画面データ
 */
int write_screen(void *scr_buf)
{
	unsigned int *buf = scr_buf;
	unsigned int *start = PKS_LCD_VRAM_START;
	unsigned int *end = PKS_LCD_VRAM_END;

	//vsync_wait();

	while (start < end)
		*start++ = *buf++;

	return 0;
}
