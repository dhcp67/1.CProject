/*************************************************************************
	> File Name: min.c
	> Author: lichun
	> Mail: 318082789@qq.com
	> Created Time: 2019年05月18日 星期六 18时04分27秒
 ************************************************************************/
#include <stdio.h>
#include "test.h"

int add(int a, int b) {
    return a + b;
}

int is_prime(int x) {
    if (x <= 1) return 0;
    for (int i = 2; i * i <= x; i++) {
        if (x % i == 0) return 0;
    }
    return 1;
}

TEST(test, is_prime_func) {
    EXPECT_EQ(is_prime(2), 0);
    EXPECT_EQ(is_prime(-2), 0);
    EXPECT_EQ(is_prime(15), 0);
    EXPECT_EQ(is_prime(9973), 1);
}

TEST(test, add_func) {
    EXPECT_EQ(add(1, 2), 3);
    EXPECT_EQ(add(3, 4), 7);
    EXPECT_EQ(add(2, 2), 4);
}

int main() {
    return RUN_ALL_TESTS();
}
