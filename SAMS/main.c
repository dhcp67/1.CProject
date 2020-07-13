/*************************************************************************
	> File Name: main.c
	> Author: Allen Lee 
	> Mail: trainlee1024@gmail.com
	> Created Time: Mon Jul 13 18:35:56 2020
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

void printscr();

//主函数
int main() {

    //调用输出函数
    printscr();
    return 0;
}

//窗口边框初始化
void winbd(int x, int y);

//输出函数
void printscr() {
    //初始化一个窗口
    WINDOW *wind = initscr();

    noecho();
    //一个字符串用来输出
    char *p = (char *)malloc(sizeof(char) * 100);

    //循环输出操作
    while (true) {
        //输入这个字符串
        getstr(p);

        //清屏
        clear();
        //绘制边框
        winbd(10,10);
        //移动到指定位置
        move(1,1);
        //添加输出到屏幕
        addstr(p);
        //刷新
        refresh();
    }

    //释放
    free(p);
    endwin();
    return ;
}

//窗口边框初始化
void winbd(int x, int y) {
    #define _addch addch('+')

    //循环边框绘制
    for(int i = 0; i < x; i++) {
        //左边框
        move(i, 0);
        _addch;
        //右边框
        move(i, 2 * y - 1);
        _addch;
    }
    for(int i = 0; i < 2 * y; i++) {
        //上边框
        move(0, i);
        _addch;
        //下边框
        move(x - 1, i);
        _addch;
    }
}
