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

#define __COLOR(a, b) "\033[" b "m" a "\033[0m"
#define COLOR(a, b) __COLOR(a, b)

#define RED(a) COLOR(a, "31")
#define GREEN(a) COLOR(a, "32")
#define YELLOW(a) COLOR(a, "33")
#define BLUE(a) COLOR(a, "34")

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

#define TYPE_STR(a) _Generic((a), \
    int : "%d", \
    double : "%lf", \
    float : "%f", \
    char : "%c", \
    const char *: "%s" \
)
#define P(a, color) { \
    char frm[100]; \
    sprintf(frm, color("%s"), TYPE_STR(a)); \
    printf(frm, a); \
}

#define EXPECT(a, b, c) { \
    __typeof(a) __temp_a = a, __temp_b = b; \
    if (__temp_a c __temp_b) add_success(); \
    else { \
        add_failed(__FILE__, __LINE__, "expect " #a " " #c " " #b); \
        P(" actual : ", YELLOW); \
        P(__temp_a, YELLOW); \
        P(" vs ", YELLOW); \
        P(__temp_b, YELLOW); \
        printf("\n\n"); \
    } \
    printf("%s %s %s %s\n", #a, #c, #b, __temp_a c __temp_b ? "True" : "False"); \
}

#define EXPECT_EQ(a, b) EXPECT(a, b, ==)
#define EXPECT_NE(a, b) EXPECT(a, b, !=)
#define EXPECT_GT(a, b) EXPECT(a, b, >)
#define EXPECT_LT(a, b) EXPECT(a, b, <)
#define EXPECT_LE(a, b) EXPECT(a, b, <=)
#define EXPECT_GE(a, b) EXPECT(a, b, >=)

typedef void (*FuncType)();

typedef struct FuncData {
    FuncType func;
    const char *name_part1;
    const char *name_part2;
    struct FuncData *next;
} FuncData, *pFuncData;

static pFuncData head = NULL;

typedef struct TotalData {
    int succ, fail;
} TotalData;

static TotalData info;

void clear_TotalData() {
    info.succ = info.fail = 0;
}

void add_success() {
    info.succ += 1;
}

void add_failed(const char *file, int line_no, const char *msg) {
    printf(YELLOW("\n%s:%d: Failure\n"), file, line_no);
    printf(YELLOW("%s"), msg);
    info.fail += 1;
}

void output_info() {
    double rate = 1.0 * info.succ / (info.succ + info.fail);
    const char *output_str = NULL;
    char output_frm[100];
    if (info.fail == 0) output_str = BLUE("%6.2lf%%");
    else output_str = RED("%6.2lf%%");
    sprintf(output_frm, "[  %s  ] total : %%3d   expect : %%3d\n", output_str);
    printf(output_frm, rate * 100, info.succ + info.fail, info.succ);
    return ;
}

int RUN_ALL_TESTS() {
    pFuncData p = head;
    while (p != NULL) {
        printf("\n[    " GREEN("RUN") "    ] %s.%s\n", p->name_part1, p->name_part2);
        clear_TotalData();
        p->func();
        p = p->next;
        output_info();
    }
    return 0;
}

#endif
