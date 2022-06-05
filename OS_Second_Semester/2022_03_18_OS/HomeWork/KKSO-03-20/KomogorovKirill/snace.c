#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <ncurses.h>
#include <pthread.h>

int work_flag = 1;
int upFlag = 0, rightFlag = 0, downFlag = 0, leftFlag = 0, ch = 0;

void heandler(int none)
{
	work_flag = 0;
	endwin();
}

static void *keyboard(void *none)
{
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	while(1)
	{
		signal(SIGINT, heandler);
		ch = getch();
		switch(ch)
		{
			case KEY_UP:
				upFlag = 1;
				rightFlag = 0;
				downFlag = 0;
				leftFlag = 0;
				break;
			case KEY_RIGHT:
				upFlag = 0;
				rightFlag = 1;
				downFlag = 0;
				leftFlag = 0;
				break;
			case KEY_DOWN:
				upFlag = 0;
				rightFlag = 0;
				downFlag = 1;
				leftFlag = 0;
				break;
			case KEY_LEFT:
				upFlag = 0;
				rightFlag = 0;
				downFlag = 0;
				leftFlag = 1;
				break;
		}
	}
	endwin();
}

int main(int argc, char *argv[])
{
	int fb, xstep, ystep;
	struct fb_var_screeninfo info;
	size_t fb_size, map_size, page_size;
	uint32_t *ptr, color, clean;


	signal(SIGINT, heandler);

	color = 0xFFFFFFFF;
	clean = 0x00000000;

	unsigned int snace[100][2];
	for (int i = 0; i < 100; i++)
	{
		snace[i][0] = 150;
		snace[i][1] = 350;
	}

	xstep = ystep = 1;
	page_size = sysconf(_SC_PAGESIZE);
	if (0 > (fb = open("/dev/fb0", O_RDWR)))
	{
		perror("open");
		return __LINE__;
	}

	if ((-1) == ioctl(fb, FBIOGET_VSCREENINFO, &info))
	{
		perror("ioctl");
		close(fb);
		return __LINE__;
	}

	fb_size = sizeof(uint32_t) * info.xres_virtual * info.yres_virtual;
	map_size = (fb_size + (page_size - 1)) & (~(page_size - 1));

	ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	if (MAP_FAILED == ptr)
	{
		perror("mmap");
		close(fb);
		return __LINE__;
	}
	
	pthread_t pid;
	int buffx = 0, buffy = 0;
	pthread_create(&pid, NULL, keyboard, NULL);
	pthread_detach(pid);
	while(work_flag)
	{
		buffx = snace[99][0];
		buffy = snace[99][1];	

		if (upFlag)
		{
			for (int i = 0; i < 100; i++)
				ptr[snace[i][1] * info.xres_virtual + snace[i][0]] = clean;
			for (int i = 0; i < 100 - 1; i++)
			{
				snace[i][0] = snace[i + 1][0];
				snace[i][1] = snace[i + 1][1];
			}
			buffy -= ystep;
			snace[99][0] = buffx;
			snace[99][1] = buffy;
			if (info.xres <= snace[99][0])
				snace[99][0] %= info.xres_virtual;

			if (info.yres <= snace[99][1])
				snace[99][1] %= info.yres_virtual;
			for (int i = 0; i < 100; i++)
				ptr[snace[i][1] * info.xres_virtual + snace[i][0]] = color;
		}

		if (rightFlag)
		{
			for (int i = 0; i < 100; i++)
				ptr[snace[i][1] * info.xres_virtual + snace[i][0]] = clean;
			for (int i = 0; i < 100 - 1; i++)
			{
				snace[i][0] = snace[i + 1][0];
				snace[i][1] = snace[i + 1][1];
			}
			buffx += xstep;
			snace[99][0] = buffx;
			snace[99][1] = buffy;
			if (info.xres <= snace[99][0])
				snace[99][0] %= info.xres_virtual;

			if (info.yres <= snace[99][1])
				snace[99][1] %= info.yres_virtual;			
			for (int i = 0; i < 100; i++)
				ptr[snace[i][1] * info.xres_virtual + snace[i][0]] = color;
		}

		if (downFlag)
		{
			for (int i = 0; i < 100; i++)
				ptr[snace[i][1] * info.xres_virtual + snace[i][0]] = clean;
			for (int i = 0; i < 100 - 1; i++)
			{
				snace[i][0] = snace[i + 1][0];
				snace[i][1] = snace[i + 1][1];
			}			
			buffy += ystep;
			snace[99][0] = buffx;
			snace[99][1] = buffy;
			if (info.xres <= snace[99][0])
				snace[99][0] %= info.xres_virtual;

			if (info.yres <= snace[99][1])
				snace[99][1] %= info.yres_virtual;			
			for (int i = 0; i < 100; i++)
				ptr[snace[i][1] * info.xres_virtual + snace[i][0]] = color;
		}

		if (leftFlag)
		{
			for (int i = 0; i < 100; i++)
				ptr[snace[i][1] * info.xres_virtual + snace[i][0]] = clean;
			for (int i = 0; i < 100 - 1; i++)
			{
				snace[i][0] = snace[i + 1][0];
				snace[i][1] = snace[i + 1][1];
			}
			buffx -= xstep;
			snace[99][0] = buffx;
			snace[99][1] = buffy;
			if (info.xres <= snace[99][0])
				snace[99][0] %= info.xres_virtual;

			if (info.yres <= snace[99][1])
				snace[99][1] %= info.yres_virtual;
			for (int i = 0; i < 100; i++)
				ptr[snace[i][1] * info.xres_virtual + snace[i][0]] = color;
		}

		usleep(200);
	}

	munmap(ptr, map_size);
	close(fb);
	return 0;
}