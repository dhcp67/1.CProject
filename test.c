/*************************************************************************
	> File Name: test.c
	> Author: lichun
	> Mail: 318082789@qq.com
	> Created Time: 2019年04月21日 星期日 18时29分22秒
 ************************************************************************/

#include <stdio.h>

int ex_gcd(int a, int b, int *x, int *y) {
    if(a == b) {
        *x = 1, *y = 0;
        return a;
    }
    int r  = ex_gcd(b, a % b, y, x);
    x= y;
    *y = *y - *x *(a / b);
    return r;
}

int main() {
    int n, m, x, y;
    while(~scanf("%d%d", &n, &m)) {
        ex_gcd(n, m, &x, &y);
        printf("%d * %d + %d * %d = %d\n",n, x, m, y, n * x + m * y);
    }    
    return 0;
}
