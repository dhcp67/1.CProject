/*************************************************************************
	> File Name: te.c
	> Author: lichun
	> Mail: 318082789@qq.com
	> Created Time: 2019年05月19日 星期日 15时22分43秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

#define pastes(a,b) a##b
#define __paste(a, b) paste(a,b)

int main() {
    int a,b;
    scanf("%d", &a);
    scanf("%d", &b);
    char str[100];
    strcpy(str,__pastes(a,b));
    printf("%s\n",str);
    return 0;
}
