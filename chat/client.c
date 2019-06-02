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

typedef struct Message{
    char from[20];
    int flags;
    char message[1024];
} Msg;

void SignalFun() {
    int status;
    wait(&status);
}

void Signalint() {
    int status;
    printf("程序已退出");
    exit(0);
}

int main() {
	int  socket_fd, new_socket;
    pid_t gip;
    pid_t sip;
    pid_t pid;
	int sport = 8731;
    int gport = 8731;
	char ip_addr[20] = "192.168.2.45";
	struct passwd *pwd;

    pwd = getpwuid(getuid());
    char umyname[20] = {0};
    strcpy(umyname, pwd->pw_name);

	struct sockaddr_in dest_addr;
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error");
	}

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(sport);
	dest_addr.sin_addr.s_addr = inet_addr(ip_addr);
    gip = fork();
    if (gip == 0) {

        if(connect(socket_fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
            perror("connect() error");
        }
        send(socket_fd, umyname, strlen(umyname), 0);
        while(1) {
             system("clear");
             char tmp[1024];
             scanf("%s", tmp);
             getchar();
             send(socket_fd, tmp, strlen(tmp), 0);
            }
    }
    struct      sockaddr_in my_addr;
	memset(     &my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(gport);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(socket_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind() error");
    }
    listen(socket_fd, 20);
    socklen_t len = sizeof(struct sockaddr);
    while(1) {
        if((new_socket = accept(socket_fd, (struct sockaddr *)&my_addr, &len)) < 0) {
            perror("accept() error");
            return 1;
        }
        pid = fork();
        if(pid > 0) {
            close(new_socket);
            continue;
        }
        pid_t ppid = getppid();
        Msg buff;
        if((recv(new_socket, &buff, sizeof(buff), 0)) > 0) {
            printf("%s", buff.from);
            printf("%d", buff.flags);
            printf("%s", buff.message);
        }
        close(new_socket);
        kill(ppid, 14);
        exit(0);
    }
	return 0;
}
