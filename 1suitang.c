/*************************************************************************
	> File Name: sui1.c
	> Author: lichun
	> Mail: 318082789@qq.com
	> Created Time: 2019年04月16日 星期二 19时35分00秒
 ************************************************************************/

#include <stdio.h>

int main() {
    // /dev/null  垃圾箱
    int n;
    char str[100];
    scanf("%d", &n);
    //FILE *fout = fopen("/dev/null", "w");
    //printf(fout,"%d", n);
    printf("%d\n",sprintf(str,"%d", n));
    return 0;
}
