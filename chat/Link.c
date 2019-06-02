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

int chac(Linkedlist linkedlist, Node *node) { //查重
    Node *p;
    p = linkedlist;
    int ret = 1;
    while (p->next != NULL) {
        if(p->next->fd == node->fd && strcmp(p->next->name, node->name) == 0) {
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
    if(!chac(linkedlist, node)) {
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
        printf("%s\n", p->name);
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
    
    Node *a = (Node *)malloc(sizeof(Node));
    a->next = NULL;
    a->fd = 1;
    strcpy(a->name,"pi11");
    insert(linkedlist, a);
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
