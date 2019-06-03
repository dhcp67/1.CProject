/*************************************************************************
	> File Name: Link.c
	> Author: lichun
	> Mail: 318082789@qq.com
	> Created Time: 2019年06月02日 星期日 15时45分22秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void output(Linkedlist linkedlist) {
    Node *p;
    p = linkedlist;
    while (p->next != NULL) {
        p = p->next;
        printf("%s\t", p->name);
    }
    return ;
}

int main() {
    Node *p = (Node *)malloc(sizeof(Node));
    p->next = NULL;
    p->fd = 0;
    strcpy(p->name,"init");
    linkedlist = p;

    Node *q = (Node *)malloc(sizeof(Node));
    q->next = NULL;
    q->fd = 1;
    strcpy(q->name,"pi11");
    insert(linkedlist, q);
    output(linkedlist);
    //chac(linkedlist, pi1)?:;

    
    p = (Node *)malloc(sizeof(Node));
    p->next = NULL;
    p->fd = 1;
    strcpy(p->name,"pi221");
    insert(linkedlist, p);
    output(linkedlist);

    Node *w = (Node *)malloc(sizeof(Node));
    w->next = NULL;
    w->fd = 1;
    strcpy(q->name,"pi123");
    insert(linkedlist, w);
    delete(linkedlist, "pi");
    output(linkedlist);

    return 0;
    
}
