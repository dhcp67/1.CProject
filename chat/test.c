/*************************************************************************
	> File Name: test.c
	> Author: lichun
	> Mail: 318082789@qq.com
	> Created Time: 2019年06月01日 星期六 20时09分48秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void Signal() {
    printf("程序已退出\n");
    _exit(0);
}

int main() {
    pid_t ppid;
    signal(SIGINT,Signal);
    ppid = fork();
    int a;
    scanf("%d", &a);
    printf("%d\n", a);
    //kill(ppid,2);
    return 0;
}
