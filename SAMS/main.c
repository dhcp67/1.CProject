/*************************************************************************
	> File Name: main.c
	> Author: Allen Lee 
	> Mail: trainlee1024@gmail.com
	> Created Time: Mon Jul 13 18:35:56 2020
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include "common.h"


//主函数
int main() {

    //调用输出函数
    printscr();
    return 0;
}

//提示函数
void tip_p(int x, int y, char *meg) {
    move(x, y);
    addstr(meg);
}

//提示结构体
struct Tip{
    char **meg;
    void (* tip_set)(int , int , char *);
};

//提示结构体初始化函数
void tip_init(int total, int length, Tip *tip) {
    tip->tip_set = tip_p;
    tip->meg = (char **)malloc(sizeof(char *) * total);
    for (int i = 0; i < total; i++) {
        tip->meg[i] = (char *)malloc(sizeof(char) * length);
    }
}

//提示结构体结束函数
void tip_end(int total, int length, Tip *tip) {
    for (int i = 0; i < total; i++) {
        free(tip->meg[i]);
    }
    free(tip->meg);
    return ;
}

//输出函数
void printscr() {
    //设置编码
    setlocale(LC_ALL,"");
    //初始化一个窗口
    WINDOW *wind = initscr();
    //定义提示结构体
    Tip tip;
    const int _TOTLE = 6, _LENGTH = 200;
    tip_init(_TOTLE, _LENGTH, &tip);
    //添加一个标题
    strcpy(tip.meg[0], "学生成绩管理系统");
    strcpy(tip.meg[1], "提示：输入q,回车退出");
    strcpy(tip.meg[2], "您的输入为 ：");
    strcpy(tip.meg[3], "正在输入ing：");
    //一个字符串用来输出
    char *p = (char *)malloc(sizeof(char) * 100);

    //循环输出操作
    while (true) {
        //绘制边框
        winbd(30, 2 * get_winsize(ROW));
        //绘制提示信息
        tip.tip_set(0, 2 * (get_winsize(ROW) - sizeof(tip.meg)), tip.meg[0]);
        tip.tip_set(2, 1, tip.meg[1]);
        tip.tip_set(3, 1, tip.meg[2]);
        tip.tip_set(4, 1, tip.meg[3]);

        //移动到指定位置
        move(1,1);
        move(4,14);
        //刷新
        refresh();

        //输入是否可见
        //echo();
        //输入这个字符串
        getstr(p);
        //输入为q时退出
        if (strcmp(p, "q") == 0) {
            break;
        }

        //清屏
        clear();
        //添加输出到屏幕
        move(3, 14);
        addstr(p);
    }

    //释放输出字符串
    free(p);
    p = NULL;
    //结束结构体
    tip_end(_TOTLE, _LENGTH, &tip);
    //结束窗口
    endwin();
    return ;
}

//窗口边框初始化
void winbd(int x, int y) {
    //宏定义添加显示字符串
    #define _addcol addstr("│");
    #define _addrow addstr("─");
    #define _mid 10
    
    for(int i = 0; i < x; i++) {
        //左边框
        move(i, 0);
        if (i == 0) {
            addstr("┌");
        } else if(i == x - 1) {
            addstr("└");
        } else if (i == _mid) {
            addstr("├");
        } else{
            _addcol;
        }
        //右边框
        move(i, 2 * y);
        if (i == 0) {
            addstr("┐");
        } else if(i == x - 1) {
            addstr("┘");
        } else if (i == _mid) {
            addstr("┤");
        } else {
            _addcol;
        }
    }

    for (int i = 1; i < y  * 2; i++) {
        //上边框
        move(0, i);
        _addrow;
        //中边框
        move(_mid, i);
        _addrow("─");
        //下边框
         move(x - 1, i);
         _addrow;
    }

    return ;
}
