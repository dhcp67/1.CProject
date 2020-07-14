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
#include <stdlib.h>
#include <curses.h>

//窗口尺寸获取
//提示信息结构体
typedef struct Tip Tip;

#define COL 1
#define ROW 2
int get_winsize(int flag);//flag：COL获取高度，ROW获取宽度

void tip_p(int x, int y, char *meg);//设置提示信息函数

//提示结构体
typedef struct Tip Tip;

//提示结构体初始化函数
void tip_init(int total, int length, Tip *tip);

//提示结构体结束函数
void tip_end(int total, int length, Tip *tip);

//添加有颜色的输出
void sta_color(int x, int y, int foreground, int background, char *p);

//输出函数
void printscr();

//窗口边框初始化函数
void winbd(int x, int y);

#endif
