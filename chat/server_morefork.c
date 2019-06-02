/*************************************************************************
	> File Name: server.c
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

void SignalFun() {
    int status;
    wait(&status);
}


int main() {
    int port = 8731;
    pid_t pid;
    pid_t pid_s;
    int sock_fd, new_sock;
    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
    }
    struct sockaddr_in my_addr;
    struct sockaddr_in peer_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind() error");
    }
    listen(sock_fd, 20);
    socklen_t len = sizeof(struct sockaddr);
    while(1) {
        if((new_sock = accept(sock_fd, (struct sockaddr *) &peer_addr, &len)) < 0) {
            perror("accept() error");
            return 1;
            exit(0);
        } 
        signal(14,SignalFun);
        pid = fork();
        if(pid >  0) {
            close(new_sock);
            continue;
        }
        pid_t ppid = getppid();
        char buff[1024] = {0};
        if((recv(new_sock, buff, sizeof(buff), 0)) > 0) {
            printf("connect success\n");
        printf("%s %s\n", buff, inet_ntoa(peer_addr.sin_addr));
        } 
            close(new_sock);
            kill(ppid, 14);
            exit(0);
    }
    close(new_sock);
    close(sock_fd);
    return 0;
}
