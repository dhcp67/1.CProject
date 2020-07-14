/*************************************************************************
	> File Name: common.h
	> Author: Allen Lee 
	> Mail: trainlee1024@gmail.com
	> Created Time: Mon Jul 13 20:37:50 2020
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#include <sys/ioctl.h>
#include <unistd.h>

//窗口尺寸获取
//提示信息结构体
typedef struct Tip Tip;

#define COL 1
#define ROW 2
int get_winsize(int flag);

//输出函数
void printscr();

//窗口边框初始化函数
void winbd(int x, int y);

#endif
