/*************************************************************************
	> File Name: test.h
	> Author: lichun
	> Mail: 318082789@qq.com
	> Created Time: 2019年05月18日 星期六 18时32分41秒
 ************************************************************************/

#ifndef _TEST_H
#define _TEST_H

#include <stdio.h>
#include <stdlib.h>

typedef void (*FuncType)();

typedef struct FuncData {
    FuncType func;
    const char *name_part1;
    const char *name_part2;
    struct FuncData *next;
} FuncData, *pFuncData;

static pFuncData head = NULL;

#define __FuncName(a, b) a##b
#define FuncName(a, b) __FuncName(a, b)
#define STR(a) #a

#define TEST(a, b) \
void FuncName(a, FuncName(_, FuncName(b, FuncName(_, Test))))(); \
__attribute__((constructor)) void FuncName(add_Test, __COUNTER__)() { \
    pFuncData p = (pFuncData)malloc(sizeof(FuncData)); \
    p->func = FuncName(a, FuncName(_, FuncName(b, FuncName(_, Test)))); \
    p->name_part1 = STR(a); \
    p->name_part2 = STR(b); \
    p->next = head; \
    head = p; \
} \
void FuncName(a, FuncName(_, FuncName(b, FuncName(_, Test))))()
#define EXPECT_EQ(a, b) printf("%s\n", a == b ? "Yes" : "No")

int RUN_ALL_TESTS() {
    pFuncData p = head;
    while (p != NULL) {
        printf("[  \033[32mRUN\033[0m     ] %s.%s\n", p->name_part1, p->name_part2);
        p->func();
        p = p->next;
        printf("[       OK ]\n");
    }
    return 0;
}

#endif
