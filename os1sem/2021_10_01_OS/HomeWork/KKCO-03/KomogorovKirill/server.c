#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdint.h>

struct data
{
	unsigned int name_len;
	unsigned int file_len;
}dt;

int sockfd = 0;

int main (int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Using: %s <number_port>\n", argv[0]);
		return 1;
	}
	int i = 0;
	unsigned int connect_count = 0, j = 0;
	FILE *file = NULL;
	char *name = NULL;
	char *buffer = NULL;
	char* remote_ip = "0.0.0.0";
	short remote_port = atoi(argv[1]);
	struct sockaddr_in remote_addr;

	int sock = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("socket creation");
		close(sockfd);
		return 2;
	}


	if (inet_pton(AF_INET, remote_ip, &remote_addr.sin_addr) <= 0) {
		perror("address parsing");
		close(sockfd);
		return 3;
	}
	remote_addr.sin_port = htons(remote_port);
	remote_addr.sin_family = AF_INET;

	if ((bind(sockfd, (struct sockaddr*)&remote_addr, sizeof(remote_addr))) < 0){
		perror("bind");
		close(sockfd);
		return 4;
	}
	if (listen(sockfd, 1) < 0)
	{
		perror("listen");
		close(sockfd);
		return 5;
	}
	while(1)
	{
		connect_count++;
		sock = accept(sockfd,NULL, sizeof(remote_addr));
		if (sock < 0)
		{
			perror("accept");
			close(sock);
			continue;
		}
		printf("----------[Connect number %d]----------\n", connect_count);
		recv(sock, &dt, sizeof(dt), 0);
		name = (char *)calloc(dt.name_len + 1, sizeof(char));
		if (name == NULL)
		{
			puts("Memory error!");
			free(name);
			close(sock);
			continue;
		}
		memset(name, '\0', dt.name_len+1);
		recv(sock, name, dt.name_len, 0);
		i = dt.name_len;
		j = 0;
		while(i >= 0)
		{
			if (name[i] == '/')
			{
				i++;
				break;
			}
			i--;
		}
		if (i != -1)
		{
			for (; i < dt.name_len; j++, i++)
				name[j] = name[i];
			for (; j < dt.name_len; j++)
				name[j] = '\0';
			dt.name_len = strlen(name);
		}
		printf("[INFO]: length name file: %d\nlength file: %d\nname file: %s\n", dt.name_len, dt.file_len, name);
		file = fopen(name, "wb");
		if (file == NULL)
		{
			puts("Error! File not found!");
			free(name);
			fclose(file);
			close(sock);
			continue;
		}
		
		buffer = (char *)calloc(dt.file_len, sizeof(char));
		if (buffer == NULL)
		{
			puts("Memory error!");
			free(name);
			fclose(file);
			free(buffer);
			close(sock);
			continue;
		}
		recv(sock, buffer, dt.file_len, 0);
		printf("[FILE CONTENTS]: %s\n", buffer);
		fwrite(buffer, sizeof(char), dt.file_len, file);
		close(sock);
		free(buffer);
		free(name);
		fclose(file);
		dt.name_len = 0;
		dt.file_len = 0;
	}
	return 0;
}

void __attribute__((destructor)) destructor(void)
{
	close(sockfd);
}
