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

struct tmpp{
    char name[20];
    int new_sock;
    pthread_t pp;
    struct Node *link;
};

typedef struct Node{
    int fd;
    char name[20];
    struct Node *next;
} Node, *Linkedlist;

void SignalFun() {
    int status;
    wait(&status);
}

Linkedlist linkedlist;

int chac(Linkedlist linkedlist, char *name) { //查重
    Node *p;
    p = linkedlist;
    int ret = 1;
    while (p->next != NULL) {
        if(strcmp(p->next->name, name) == 0) {
            ret = 0;
            printf("用户名重复\n不能登录\n");
            break;
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

void output(Linkedlist linkedlist) {
    Node *p;
    p = linkedlist;
    if (p == NULL) {
        printf("NULL\n");
        return ;
    }
    printf("在线用户:\n");
    int cnt = 0;
    while (p->next != NULL) {
        p = p->next;
        cnt++;
        printf("%s  \n", p->name);
    }
    printf("一共%d人在线\n", cnt);
    return;
}

void *pth(void *arg) {
    struct tmpp *tmp;
    tmp = (struct tmpp *)arg;
    int new_sock;
    new_sock = tmp->new_sock;
    char buff[1024] = {0};

    Node *q = (Node *)malloc(sizeof(Node));
    if((recv(new_sock, buff, sizeof(buff), 0)) > 0) {
        strcpy(q->name, buff);
        printf("%s 已上线\n", q->name);
        //ip地址：printf("%s",inet_ntoa(peer_addr.sin_addr));
        q->next = NULL;
        q->fd = tmp->new_sock;
        insert(tmp->link, q);
        output(tmp->link);
    }
    while(1){
        if((recv(new_sock, buff, sizeof(buff), 0)) > 0) {
            printf("%s %s\n", q->name,buff);
        }else{
            printf("%s已下线\n", q->name);
            delete(linkedlist, q->name);
            close(tmp->new_sock);
            output(linkedlist);
            return (void *)0;
        } 
    }
}

int main() {
    int port = 8731;
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
    int cnt = 0;
    pthread_t pp[100];

    while(++cnt && cnt < 100) {
        if((new_sock = accept(sock_fd, (struct sockaddr *) &peer_addr, &len)) < 0) {
            perror("accept() error");
            continue;
        }
        struct tmpp ooo;
        ooo.new_sock = new_sock;
        ooo.link = linkedlist;
        pthread_create(&(pp[cnt]), NULL, pth, (void *)&ooo);
    }
    return 0;
}
