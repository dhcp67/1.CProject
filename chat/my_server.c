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
#include <pthread.h>

void SignalFun() {
    int status;
    wait(&status);
}


typedef struct Node{
    int fd;
    char name[20];
    struct Node *next;
} Node, *Linkedlist;

Linkedlist linkedlist;

int chac(Linkedlist linkedlist, char *name) { //查重
    Node *p;
    p = linkedlist;
    int ret = 1;
    while (p->next != NULL) {
        if(strcmp(p->next->name, name) == 0) {
            ret = 0;
            printf("重复\n不能登录\n");
        }
        p = p->next;
    }
    return ret;
}

void insert(Linkedlist linkedlist, Node *node) {
    Node *p;
    p = linkedlist;
    if(!chac(linkedlist, node->name)) {
        return ;
    }
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = node;
    return ;
}


Linkedlist CreateNode(int fd, char *name) {
    Node *p = (Node *)malloc(sizeof(Node));
    p->next = NULL;
    p->fd = fd;
    strcpy(p->name, name);
    return p;
}


void delete(Linkedlist linkedlist, const char *name) {
    Node *p, * q;
    p  = linkedlist;
    while (p->next != NULL && strcmp(p->next->name, name)) {
        p = p->next;
    }
    if (p->next == NULL) return ;
    q = p->next;
    p->next = q->next;
    free(q);
}

void *output(Linkedlist linkedlist) {
    Node *p;
    p = linkedlist;
    while (p->next != NULL) {
        p = p->next;
        printf("%s\t", p->name);
    }
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
    
    Node *p = (Node *)malloc(sizeof(Node));
    p->next = NULL;
    p->fd = 0;
    strcpy(p->name,"super man:");
    linkedlist = p;

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
        char username[20];
        if((recv(new_sock, buff, sizeof(buff), 0)) > 0) {
            printf("%s 已上线\n", buff);
            //ip地址：printf("%s",inet_ntoa(peer_addr.sin_addr));
            Node *q = (Node *)malloc(sizeof(Node));

            q->next = NULL;
            q->fd = new_sock;
            strcpy(q->name,buff);
            output(linkedlist);
        }else{
            sleep(1);
            printf("%s已下线\n", username);
            delete(linkedlist, username);
            output(linkedlist);
        } 
            
            close(new_sock);
            kill(ppid, 14);
            exit(0);
    }
    close(new_sock);
    close(sock_fd);
    return 0;
}
