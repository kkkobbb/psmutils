#define TRUE	1
#define FALSE	0

#define PAD_BUTTON	(1 << 0)
#define PAD_RIGHT	(1 << 1)
#define PAD_LEFT	(1 << 2)
#define PAD_DOWN	(1 << 3)
#define PAD_UP		(1 << 4)
#define PAD_ALL		(PAD_BUTTON | PAD_RIGHT | PAD_LEFT | PAD_DOWN | PAD_UP)

#define LCD_SIZE 32
#define ONE_SEC	32


typedef void (*funcp)();
struct save_data {
	int highscore;
};
unsigned int lcd[LCD_SIZE];
extern struct save_data save_data;
