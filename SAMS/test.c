/*************************************************************************
	> File Name: test.c
	> Author: Allen Lee 
	> Mail: trainlee1024@gmail.com
	> Created Time: Mon Jul 13 20:54:11 2020
 ************************************************************************/

#include <stdio.h>
#include "common.h"

int main() {
    printf("COL = %d, ROW = %d\n", get_winsize(COL), get_winsize(ROW));
}
