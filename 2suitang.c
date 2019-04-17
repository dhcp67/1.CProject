/*************************************************************************
	> File Name: suitang2.c
	> Author: lichun
	> Mail: 318082789@qq.com
	> Created Time: 2019年04月16日 星期二 20时45分27秒
 ************************************************************************/

#include <stdio.h>

int main() {
    // ~
    //[^\n]
    

    int i = 0;
    char str[100];
    scanf("%[^\n]s", str);
    getchar();
    int ret = printf("%s", str);
    printf("%d\n", ret);
    return 0;
}
