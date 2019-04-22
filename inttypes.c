/************************************************************************
	> File Name: test.c
	> Author: lichun
	> Mail: 318082789@qq.com
	> Created Time: 2019年04月18日 星期四 18时41分30秒
 ************************************************************************/

#include <stdio.h>
#include <inttypes.h>

int main() {
    int8_t a;
    int16_t b;
    int32_t c;
    int64_t d;
    scanf("%" PRId64, &d);
    printf("%" PRId64"\n", d);
    printf("PRId8 = %s""\n", PRId8);
    printf("PRId16 = %s\n", PRId16);
    printf("PRId32 = %s\n", PRId32);
    printf("PRId64 = %s\n", PRId64);
    printf("%" PRIX64"\n", INT64_MIN);
    printf("%" PRIX64"\n", INT64_MAX);
    return 0;
}
