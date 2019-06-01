/*************************************************************************
	> File Name: messge.c
	> Author: 
	> Mail: 
	> Created Time: 2019年04月13日 星期六 14时12分54秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <wait.h>
#include <signal.h>

typedef struct Node{
    int fd;
    char name[20];
    struct Node *next;
} Node;

int socket_connect(int port, char *host) {
	int sockfd;
	struct sockaddr_in dest_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error");
		return -1;
	}

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(host);

	if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
		perror("connect() error");
		return -1;
	}
	return sockfd;

}

void SignalFun() {
    int status;
    wait(&status);
}

int main() {
	int  socket_fd, new_socket;
    pid_t gip;
    pid_t sip;
    pid_t pid;
	int sport = 8371;
    int gport = 8372;
	char ip_addr[20] = "192.168.2.45";
	struct passwd *pwd;
    struct sockaddr_in ser_addr;
    pwd = getpwuid(getuid());
    char username[20] = {0};
    strcpy(username, pwd->pw_name);
    gip = fork();
    if (gip == 0) {
        if ((socket_fd = socket_connect(sport, ip_addr)) > 0) {
            printf("OK\n");
        }
        send(socket_fd, username, strlen(username), 0);
	    close(socket_fd);
    } else {
       if ((socket_fd = socket_connect(gport, ip_addr)) < 0) {
           perror("sockf_fd error");
        }
        listen(socket_fd, 20);
        socklen_t len = sizeof(struct sockaddr);
        while(1) {
            if((new_socket = accept(socket_fd, (struct sockaddr *)&ser_addr, &len)) < 0) {
                perror("accept() error");
                return 1;
            }
            pid = fork();
            if(pid > 0) {
                close(new_socket);
                continue;
            }
            pid_t ppid = getppid();
            struct Node buff;
            if((recv(new_socket, &buff, sizeof(buff), 0)) > 0) {
                printf("%d", buff.fd);
            }
            close(new_socket);
            kill(ppid, 14);
            exit(0);
        }
        
    }
	return 0;

}
