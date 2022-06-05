#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <fcntl.h>
#include <signal.h>
#include <ncurses.h>
#include <pthread.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#define INTERFACE "eno1"
#define PORT 12345

struct TronPlayer
{
	int x;
	int y;
	uint32_t color;
} local, rem;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int fb, xstep, ystep, deadFlag = 1;
struct fb_var_screeninfo info;
size_t fb_size, map_size, page_size;
uint32_t* ptr;
int xres = 0, yres = 0;
int buffx = 0, buffy = 0, bufx = 0, bufy = 0;
int work_flag = 1, cd = 0;
int upLoc = 0, rightLoc = 0, downLoc = 0, leftLoc = 0;
char* ip2 = NULL;
int flag = 1;

struct sockaddr_in server_struct, client_struct, other;
int server, client, i, slen = sizeof(client_struct), recv_len, send_len;
char key_rem = '\0', ch = '\0', brem = '\0';
char prev_key = '\0', rem_prev_key = '\0';
int *DEADREM, *DEADLOC;
int winred = 0, winblue = 0;
int start = 1;
int u = 1, d = 1, l = 1, r = 1;
char buff;
int dead = 0;

void heandler(int none)
{
	work_flag = 0;
	endwin();
	close(cd);
	munmap(ptr, map_size);
	close(client);
	close(server);
	exit(0);
}

void die(char* s)
{
	perror(s);
	munmap(ptr, map_size);
	endwin();
	exit(1);
}

void create_tron(int x1, int y1, int x2, int y2, uint32_t border_color, int solid)
{
	for (int i = x1; i <= x2; i++)
	{
		if ((y1 * info.xres_virtual + i) < 0)
		{
			if (border_color == 0x00FF0000)
				winblue = 1;
			else
				winred = 1;
			dead = 1;
			return;
		}
		ptr[y1 * info.xres_virtual + i] = border_color;
		if ((y2 * info.xres_virtual + i) < 0)
		{
			if (border_color == 0x00FF0000)
				winblue = 1;
			else
				winred = 1;
			dead = 1;
			return;
		}
		ptr[y2 * info.xres_virtual + i] = border_color;
	}
	for (int i = y1; i <= y2; i++) 
	{
		if (((i * info.xres_virtual + x1) < 0) || ((i * info.xres_virtual + x2) < 0))
		{
			if (border_color == 0x00FF0000)
				winblue = 1;
			else
				winred = 1;
			dead = 1;
			return;
		}
		ptr[i * info.xres_virtual + x1] = border_color;
		ptr[i * info.xres_virtual + x2] = border_color;
	}
	if (solid)
		for (i = x1; i < x2; i++)
			for (int j = y1; j < y2; j++)
				ptr[j * info.xres_virtual + i] = border_color;
}

void diedron(int x1, int y1, int x2, int y2, int bx1, int by1, int bx2, int by2, uint32_t loose)
{
	if ((x1 == bx1) && (y1 == by1) || (x2 == bx2) && (y2 == by2))
	{
		buff = 'q';
		sendto(client, &buff, sizeof(char), 0, (struct sockaddr*)&client_struct, slen);
		winred = 1;
		winblue = 1;
		flag = 1;
		return;
	}
	if ((y1 <= 0 || y2 <= 0 || x1 <= 0 || x2 <= 0) || (y1 >= yres || y2 >= yres || x1 >= xres || x2 >= xres))
	{
		sendto(client, &ch, sizeof(char), 0, (struct sockaddr*)&client_struct, slen);
		if (loose == 0x00FF0000)
			winblue = 1;
		else
		    	winred = 1;
		flag = 1;
	 }

	 for (int i = x1; i <= x2; i++)
	 	for (int j = y1; j <= y2; j++)
			if (j >= 0)
			{
				if (ptr[j * info.xres_virtual + i] == 0xFFFFFFFF || ptr[j * info.xres_virtual + i] == local.color || ptr[j * info.xres_virtual + i] == rem.color)
		    		{	
					sendto(client, &ch, sizeof(char), 0, (struct sockaddr*)&client_struct, slen);
		    			if (loose == 0x00FF0000)
		    				winblue = 1;
		    			else
		    				winred = 1;
					flag = 1;
		    		}
			}
	 		else
			{
				winblue = 1;
				flag = 1;
			}
	 
	 /*for (int i = x1, j = bx1; i <= bx1, j <= bx2; i++, j++)
		 for (int k = y1, l = by1; k <= by1, l <= by2; k++, l++)
			if ((ptr[k * info.xres_virtual + i] == rem.color && ptr[l * info.xres_virtual + j] == local.color) || (ptr[k * info.xres_virtual + i] == local.color && ptr[l * info.xres_virtual + j] == rem.color))
			{
				sendto(client, &ch, sizeof(char), 0, (struct sockaddr*)&client_struct, slen);
				winblue = 1;
				winred = 1;
				flag = 1;
			}*/

}

static void* keyboard(void* none)
{
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	while (1)
	{
		signal(SIGINT, heandler);
		ch = getch();
		if (ch == 'w' && u && flag)
		{
			upLoc = 1;
			rightLoc = 0;
			downLoc = 0;
			leftLoc = 0;
			l = 1;
			r = 1;
			u = 1;
			d = 0;
		}

		if (ch == 'd' && r && flag)
		{
			upLoc = 0;
			rightLoc = 1;
			downLoc = 0;
			leftLoc = 0;
			r = 1;
			u = 1;
			d = 1;
			l = 0;
		}

		if (ch == 's' && d && flag)
		{
			upLoc = 0;
			rightLoc = 0;
			downLoc = 1;
			leftLoc = 0;
			r = 1;
			d = 1;
			l = 1;
			u = 0;

		}

		if (ch == 'a' && l && flag)
		{
			upLoc = 0;
			rightLoc = 0;
			downLoc = 0;
			leftLoc = 1;
			u = 1;
			d = 1;
			r = 1;
			l = 0;
		}
	}
	endwin();
}

static void* field(void* none)
{
	while (work_flag)
	{
		for (int i = 0; i < xres; i++)
		{
			ptr[0 * info.xres_virtual + i] = 0xFFFFFFFF;
			ptr[(yres - 1) * info.xres_virtual + i] = 0xFFFFFFFF;
		}

		for (int i = 0; i < yres; i++)
		{
			ptr[i * info.xres_virtual] = 0xFFFFFFFF;
			ptr[i * info.xres_virtual + (xres - 1)] = 0xFFFFFFFF;
		}
	}
}

static void* play(void *none)
{
	while (work_flag)
	{
	    	send_len = sendto(client, &ch, sizeof(char), 0, (struct sockaddr*)&client_struct, slen);
        	if (send_len == -1)
        	{
            		if (local.color == 0x00FF0000)
              			winblue = 1;
            		else
              			winred = 1;
        	}
		brem = key_rem;
		if ((recv_len = recvfrom(server, &key_rem, sizeof(char), 0, (struct sockaddr*)&other, &slen)) == 0)
			key_rem = brem;
        	if (recv_len == -1)
        	{
            		if (rem.color == 0x00FF0000)
              			winblue = 1;
            		else
              			winred = 1;
        	}
		if ((other.sin_addr.s_addr != inet_addr(ip2)) || (rem_prev_key == 'w' && key_rem == 's') || (rem_prev_key == 's' && key_rem == 'w') || (rem_prev_key == 'a' && key_rem == 'd') || (rem_prev_key == 'd' && key_rem == 'a'))
			key_rem = brem;
		pthread_mutex_lock(&mutex);
		if (key_rem != '\0' && ch != '\0' && start)
		{
			start = 0;
			if (local.color == 0x00FF0000)
				ch = 'd';
			else
				ch = 'a';
				if (ch == 'w' && flag)
				{
		    			upLoc = 1;
					rightLoc = 0;
					downLoc = 0;
					leftLoc = 0;
				}
		  		if (ch == 'd' && flag)
				{
					upLoc = 0;
					rightLoc = 1;
					downLoc = 0;
					leftLoc = 0;
				}
				if (ch == 's' && flag)
				{
					upLoc = 0;
					rightLoc = 0;
					downLoc = 1;
					leftLoc = 0;
				}
				if (ch == 'a' && flag)
				{
					upLoc = 0;
					rightLoc = 0;
					downLoc = 0;
					leftLoc = 1;
				}
			sendto(client, &ch, sizeof(char), 0, (struct sockaddr*)&client_struct, slen);
			recv_len = recvfrom(server, &key_rem, sizeof(char), 0, (struct sockaddr*)&other, &slen);
			if (other.sin_addr.s_addr != inet_addr(ip2))
			{
				ch = '\0';
				key_rem = '\0';
				start = 1;
			}		
		}
		bufx = rem.x;
		bufy = rem.y;
		buffx = local.x;
		buffy = local.y;

		if ((key_rem == 'w') && !(*DEADREM) && !start)
		{
				if (rem_prev_key == 'a')
					create_tron(rem.x - 8, rem.y - 2, rem.x - 1, rem.y + 2, 0, 1);
				if (rem_prev_key == 's')
					create_tron(rem.x - 2, rem.y + 1, rem.x + 2, rem.y + 8, 0, 1);
				if (rem_prev_key == 'd')
					create_tron(rem.x + 1, rem.y - 2, rem.x + 8, rem.y + 2, 0, 1);
				if (rem_prev_key != 'w')
					diedron(rem.x - 2, rem.y - 8, rem.x + 2, rem.y - 1, local.x - 2, local.y + 1, local.x + 2, local.y + 8, rem.color);
				if (!(*DEADREM))
				{
					rem_prev_key = 'w';
					create_tron(rem.x - 2, rem.y - 8, rem.x + 2, rem.y - 1, 0, 1);
	
					bufy -= ystep;
					rem.x = bufx;
					rem.y = bufy;
					
					ptr[rem.y * info.xres_virtual + rem.x] = rem.color;
					diedron(rem.x - 2, rem.y - 8, rem.x + 2, rem.y - 1, local.x - 2, local.y + 1, local.x + 2, local.y + 8, rem.color);
					create_tron(rem.x - 2, rem.y - 8, rem.x + 2, rem.y - 1, rem.color, 1);
				}
		}
		
		if (upLoc && !(*DEADLOC) && !start)
        	{
			flag = 0;
		    	if (prev_key == 'a')
				create_tron(local.x - 8, local.y - 2, local.x - 1, local.y + 2, 0, 1);
			if (prev_key == 's')
				create_tron(local.x - 2, local.y + 1, local.x + 2, local.y + 8, 0, 1);
			if (prev_key == 'd')
				create_tron(local.x + 1, local.y - 2, local.x + 8, local.y + 2, 0, 1);
			if (prev_key != 'w')
				diedron(local.x - 2, local.y - 8, local.x + 2, local.y - 1, rem.x - 2, rem.y + 1, rem.x + 2, rem.y + 8, local.color);
			if (!(*DEADLOC))
			{
				prev_key = 'w';
				create_tron(local.x - 2, local.y - 8, local.x + 2, local.y - 1, 0, 1);
	
				buffy -= ystep;
		    		local.x = buffx;
				local.y = buffy;
				
				ptr[local.y * info.xres_virtual + local.x] = local.color;
				diedron(local.x - 2, local.y - 8, local.x + 2, local.y - 1, rem.x - 2, rem.y + 1, rem.x + 2, rem.y + 8, local.color);
				create_tron(local.x - 2, local.y - 8, local.x + 2, local.y - 1, local.color, 1);
			}
			flag = 1;
		}

		if ((key_rem == 'd') && !(*DEADREM) && !start)
		{
			if (rem_prev_key == 'a')
				create_tron(rem.x - 8, rem.y - 2, rem.x - 1, rem.y + 2, 0, 1);
			if (rem_prev_key == 's')
				create_tron(rem.x - 2, rem.y + 1, rem.x + 2, rem.y + 8, 0, 1);
			if (rem_prev_key == 'w')
				create_tron(rem.x - 2, rem.y - 8, rem.x + 2, rem.y - 1, 0, 1);
			if (rem_prev_key != 'd')
				diedron(rem.x + 1, rem.y - 2, rem.x + 8, rem.y + 2, local.x - 8, local.y - 2, local.x - 1, local.y + 2, rem.color);
			if (!(*DEADREM))
			{
				rem_prev_key = 'd';
				create_tron(rem.x + 1, rem.y - 2, rem.x + 8, rem.y + 2, 0, 1);

				bufx += xstep;
				rem.x = bufx;
				rem.y = bufy;

				ptr[rem.y * info.xres_virtual + rem.x] = rem.color;
				diedron(rem.x + 1, rem.y - 2, rem.x + 8, rem.y + 2, local.x - 8, local.y - 2, local.x - 1, local.y + 2, rem.color);
				create_tron(rem.x + 1, rem.y - 2, rem.x + 8, rem.y + 2, rem.color, 1);
			}
		}

		if (rightLoc && !(*DEADLOC) && !start)
		{
			flag = 0;
			if (prev_key == 'a')
				create_tron(local.x - 8, local.y - 2, local.x - 1, local.y + 2, 0, 1);
			if (prev_key == 's')
				create_tron(local.x - 2, local.y + 1, local.x + 2, local.y + 8, 0, 1);
			if (prev_key == 'w')
				create_tron(local.x - 2, local.y - 8, local.x + 2, local.y - 1, 0, 1);
			if (prev_key != 'd')
				diedron(local.x + 1, local.y - 2, local.x + 8, local.y + 2, rem.x - 8, rem.y - 2, rem.x - 1, rem.y + 2, local.color);
			if (!(*DEADLOC))
			{
				prev_key = 'd';
				create_tron(local.x + 1, local.y - 2, local.x + 8, local.y + 2, 0, 1);

				buffx += xstep;
				local.x = buffx;
				local.y = buffy;
				
				ptr[local.y * info.xres_virtual + local.x] = local.color;
				diedron(local.x + 1, local.y - 2, local.x + 8, local.y + 2, rem.x - 8, rem.y - 2, rem.x - 1, rem.y + 2, local.color);
				create_tron(local.x + 1, local.y - 2, local.x + 8, local.y + 2, local.color, 1);
			}
			flag = 1;
	
		}
		
		if ((key_rem == 's') && !(*DEADREM) && !start)
		{
			if (rem_prev_key == 'a')
				create_tron(rem.x - 8, rem.y - 2, rem.x - 1, rem.y + 2, 0, 1);
			if (rem_prev_key == 'w')
				create_tron(rem.x - 2, rem.y - 8, rem.x + 2, rem.y - 1, 0, 1);
			if (rem_prev_key == 'd')
				create_tron(rem.x + 1, rem.y - 2, rem.x + 8, rem.y + 2, 0, 1);
			if (rem_prev_key != 's')
				diedron(rem.x - 2, rem.y + 1, rem.x + 2, rem.y + 8, local.x - 2, local.y - 8, local.x + 2, local.y - 1, rem.color);
			if (!(*DEADREM))
			{
				rem_prev_key = 's';
				
				create_tron(rem.x - 2, rem.y + 1, rem.x + 2, rem.y + 8, 0, 1);

				bufy += ystep;
				rem.x = bufx;
				rem.y = bufy;
				
				ptr[rem.y * info.xres_virtual + rem.x] = rem.color;
				diedron(rem.x - 2, rem.y + 1, rem.x + 2, rem.y + 8, local.x - 2, local.y - 8, local.x + 2, local.y - 1, rem.color);
				create_tron(rem.x - 2, rem.y + 1, rem.x + 2, rem.y + 8, rem.color, 1);
			}
		}

		if (downLoc && !(*DEADLOC) && !start)
		{
			flag = 0;
			if (prev_key == 'a')
				create_tron(local.x - 8, local.y - 2, local.x - 1, local.y + 2, 0, 1);
			if (prev_key == 'w')
				create_tron(local.x - 2, local.y - 8, local.x + 2, local.y - 1, 0, 1);
			if (prev_key == 'd')
				create_tron(local.x + 1, local.y - 2, local.x + 8, local.y + 2, 0, 1);
			if (prev_key != 's')
				diedron(local.x - 2, local.y + 1, local.x + 2, local.y + 8, rem.x - 2, rem.y - 8, rem.x + 2, rem.y - 1, local.color);
			if (!(*DEADLOC))
			{
				prev_key = 's';
				create_tron(local.x - 2, local.y + 1, local.x + 2, local.y + 8, 0, 1);

				buffy += ystep;
				local.x = buffx;
				local.y = buffy;
		
				ptr[local.y * info.xres_virtual + local.x] = local.color;
				diedron(local.x - 2, local.y + 1, local.x + 2, local.y + 8, rem.x - 2, rem.y + 1, rem.x + 2, rem.y + 8, local.color);
				create_tron(local.x - 2, local.y + 1, local.x + 2, local.y + 8, local.color, 1);
			}
			flag = 1;

		}
			
		if ((key_rem == 'a') && !(*DEADREM) && !start)
		{
			if (rem_prev_key == 'w')
				create_tron(rem.x - 2, rem.y - 8, rem.x + 2, rem.y - 1, 0, 1);
			if (rem_prev_key == 's')
				create_tron(rem.x - 2, rem.y + 1, rem.x + 2, rem.y + 8, 0, 1);
			if (rem_prev_key == 'd')
				create_tron(rem.x + 1, rem.y - 2, rem.x + 8, rem.y + 2, 0, 1);
			if (rem_prev_key != 'a')
				diedron(rem.x - 8, rem.y - 2, rem.x - 1, rem.y + 2, local.x + 1, local.y - 2, local.x + 8, local.y + 2, rem.color);
			if (!(*DEADREM))
			{
				rem_prev_key = 'a';
				create_tron(rem.x - 8, rem.y - 2, rem.x - 1, rem.y + 2, 0, 1);

				bufx -= xstep;
				rem.x = bufx;
				rem.y = bufy;

				ptr[rem.y * info.xres_virtual + rem.x] = rem.color;
				diedron(rem.x - 8, rem.y - 2, rem.x - 1, rem.y + 2, local.x + 1, local.y - 2, local.x + 8, local.y + 2, rem.color);
				create_tron(rem.x - 8, rem.y - 2, rem.x - 1, rem.y + 2, rem.color, 1);
			}

		}

		if (leftLoc && !(*DEADLOC) && !start)
		{
			flag = 0;
			if (prev_key == 'w')
				create_tron(local.x - 2, local.y - 8, local.x + 2, local.y - 1, 0, 1);
			if (prev_key == 's')
				create_tron(local.x - 2, local.y + 1, local.x + 2, local.y + 8, 0, 1);
			if (prev_key == 'd')
				create_tron(local.x + 1, local.y - 2, local.x + 8, local.y + 2, 0, 1);
			if (prev_key != 'a')
				diedron(local.x - 8, local.y - 2, local.x - 1, local.y + 2, rem.x + 1, rem.y - 2, rem.x + 8, rem.y + 2, local.color);
			if (!(*DEADLOC))
			{
				prev_key = 'a';
				
				create_tron(local.x - 8, local.y - 2, local.x - 1, local.y + 2, 0, 1);

				buffx -= xstep;
				local.x = buffx;
				local.y = buffy;

				ptr[local.y * info.xres_virtual + local.x] = local.color;
				diedron(local.x - 8, local.y - 2, local.x - 1, local.y + 2, rem.x + 1, rem.y - 2, rem.x + 8, rem.y + 2, local.color);
				create_tron(local.x - 8, local.y - 2, local.x - 1, local.y + 2, local.color, 1);
			}
			flag = 1;
		}
		usleep(62500);
		/*brem = key_rem;
		if ((recv_len = recvfrom(server, &key_rem, sizeof(char), 0, (struct sockaddr*)&other, &slen)) == 0)
			key_rem = brem;
		if (other.sin_addr.s_addr != inet_addr(ip2) || key_rem == 'w' || key_rem == 'd' || key_rem == 's' || key_rem == 'a')
			key_rem = brem;*/
			
		if (winblue && winred || key_rem == 'q')
		{
			mvprintw(LINES / 2, COLS / 2, "Draw");
			refresh();
		    	sleep(5);
			heandler(90);
		}
		
		if (winblue && !winred && dead)
		{
			mvprintw(LINES / 2, COLS / 2, "Win Blue");
			refresh();
		    	sleep(5);
			heandler(90);
		}
		
		if (!winblue && winred && dead)
		{
			mvprintw(LINES / 2, COLS / 2, "Win Red");
			refresh();
		    	sleep(5);
			heandler(90);
		}
		if (winred || winblue)
			dead = 1;

		//usleep(62500);
		pthread_mutex_unlock(&mutex);
	}
	munmap(ptr, map_size);
	close(fb);
	close(client);
	close(server);
}

long int convIP(char* ip)
{
	long int altIP = 0;
	char* tmp = strtok(ip, ".");
	if (NULL == tmp)
	{
		puts("Error!");
		return 1;
	}

	int i = 1;
	while (tmp != NULL)
	{
		altIP += atoi(tmp) * pow(256, 4 - i);
		i++;
		tmp = strtok(NULL, ".");
	}
	return altIP;
}

int main(int argc, char* argv[])
{
	int flagIP;
	if (argc != 3)
	{
		printf("Using: %s <size_screen> <ip_opponent>\n", argv[0]);
		return 1;
	}

	struct ifreq ifr;
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strcpy(ifr.ifr_name, INTERFACE);
	ioctl(s, SIOCGIFADDR, &ifr);
	close(s);
	struct sockaddr_in* sa = (struct sockaddr_in*)&ifr.ifr_addr;
	char* ip1 = NULL;

	asprintf(&ip1, "%s", inet_ntoa(sa->sin_addr));
	if (NULL == ip1)
	{
		puts("Memory error!");
		return 1;
	}

	asprintf(&ip2, "%s", argv[2]);
	if (NULL == ip2)
	{
		puts("Memory error!");
		return 1;
	}
	
	if (!(strcmp(ip1, ip2)))
		heandler(90);
	long int altIP1 = convIP(ip1);
	long int altIP2 = convIP(argv[2]);

	if (altIP1 > altIP2)
		flagIP = 0;
	else
		flagIP = 1;
	free(ip1);



	char* split = "xX", * tmp;

	tmp = strtok(argv[1], split);
	if (NULL == tmp)
	{
		puts("Error! Incorrect screen size value!");
		return 1;
	}
	xres = atoi(tmp);
	tmp = strtok(NULL, split);
	if (NULL == tmp)
	{
		puts("Error! Incorrect screen size value!");
		return 1;
	}
	yres = atoi(tmp);
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

	if (flagIP)
	{
		local.x = xres - 1;
		local.y = yres - 4;
		local.color = 0x000000FF;
		DEADLOC = &winred;

		rem.x = 0;
		rem.y = 3;
		rem.color = 0x00FF0000;
		DEADREM = &winblue;
	}
	else
	{
		local.x = 0;
		local.y = 3;
		local.color = 0x00FF0000;
		DEADLOC = &winblue;

		rem.x = xres - 1;
		rem.y = yres - 4;
		rem.color = 0x000000FF;
		DEADREM = &winred;
	}

	if ((server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket");
	memset((char*)&server_struct, 0, sizeof(server_struct));
	server_struct.sin_family = AF_INET;
	server_struct.sin_port = htons(PORT);
	server_struct.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if ((client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket()");
	client_struct.sin_family = AF_INET;
	client_struct.sin_port = htons(PORT);
	if (inet_aton(ip2, &client_struct.sin_addr) == 0)
		die("inet_aton()");

	if (bind(server, (struct sockaddr*)&server_struct, sizeof(server_struct)))
		die("bind");

	pthread_t players, pid, fid;
	pthread_create(&fid, NULL, field, NULL);
	pthread_create(&pid, NULL, keyboard, NULL);
	pthread_create(&players, NULL, play, NULL);
	pthread_detach(pid);
	pthread_join(fid, NULL);
   	pthread_join(players, NULL);
	return 0;
}
