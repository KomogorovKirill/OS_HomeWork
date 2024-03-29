#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>

struct data
{
	unsigned int name_len;
	unsigned int file_len;
}dt;

int main(int argc, char **argv)
{
    if (argc != 4)
    {
	    printf("Using: %s <ip_address> <port> <path_to_file>\n", argv[0]);
	    return 1;
    }

    struct stat buff;
    char name[1025] = {'\0'};
    char remote_ip[15] = {'\0'};
    strncpy(remote_ip, argv[1], strlen(argv[1]));
    short remote_port = atoi(argv[2]);
    
    struct sockaddr_in remote_addr;
    
    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0)
    {
	    perror("socket creation");
	    close(sockfd);
            return 2;
    }
    
    if (inet_pton(AF_INET, remote_ip, &remote_addr.sin_addr) <= 0){
            perror("address parsing");
	    close(sockfd);
            return 3;
    }
    remote_addr.sin_port = htons(remote_port);
    remote_addr.sin_family = AF_INET;
    
    if (connect(sockfd, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) < 0 )
    {
            perror("connect");
	    close(sockfd);
            return 4;
    }
    stat(argv[3], &buff); 
    dt.name_len = strlen(argv[3]);
    dt.file_len = buff.st_size;
    strncpy(name, argv[3], dt.name_len);
    send(sockfd, &dt, sizeof(dt), 0);
    send(sockfd, name, dt.name_len, 0);
    FILE *file = fopen(argv[3], "rb");
    if (NULL == file)
    {
	    puts("Error! File not found!");
	    fclose(file);
	    close(sockfd);
	    return 5;
    }
    char *buffer = (char *)calloc(dt.file_len, sizeof(char));
    if (NULL == buffer)
    {
	    puts("Memory error!");
	    free(buffer);
	    fclose(file);
	    close(sockfd);
	    return 6;
    }
    for (unsigned int i = 0; i < dt.file_len; i++)
	buffer[i] = fgetc(file);
     
    send(sockfd, buffer, dt.file_len, 0);

    free(buffer);
    fclose(file);
    close(sockfd);
    return 0;
}

