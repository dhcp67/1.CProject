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

int main() {                              //主函数

    printscr();                           //调用输出函数
    return 0;
}

struct Tip{                               //提示结构体
    char **meg;
    void (* tip_set)(int , int , char *);
};

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

    setlocale(LC_ALL,"");                //设置编码
    WINDOW *wind = initscr();            //初始化一个窗口
    Tip tip;                             //定义提示结构体
    const int _TOTLE = 6, _LENGTH = 200;
    tip_init(_TOTLE, _LENGTH, &tip);
    strcpy(tip.meg[0], "学生成绩管理系统");          //添加一个标题
    strcpy(tip.meg[1], "提示(回车结束输入)：输入q：退出");
    strcpy(tip.meg[2], "您的输入为 ：");
    strcpy(tip.meg[3], "正在输入ing：");
    char *p = (char *)malloc(sizeof(char) * 100);   //一个字符串用来输出
    
    while (true) {                                  //循环输出操作
        
        winbd(30, 2 * get_winsize(ROW));            //绘制边框
        tip.tip_set(0, 2 * (get_winsize(ROW) - sizeof(tip.meg)), tip.meg[0]);//绘制提示信息
        //绘制有颜色的提示信息
		if(start_color()==OK) {                     //判断终端是否支持有颜色输出
		    sta_color(2, 1, COLOR_RED, COLOR_BLACK, tip.meg[1]);
        }
		else
		{
            tip.tip_set(2, 1, tip.meg[1]);
		}
        
        //tip.tip_set(2, 1, tip.meg[1]);
        tip.tip_set(3, 1, tip.meg[2]);
        tip.tip_set(4, 1, tip.meg[3]);

        move(1,1);    //移动到指定位置
        move(4,14);
        
        refresh();    //刷新

        //echo();     //输入是否可见
        getstr(p);    //输入这个字符串
        //输入为q时退出
        if (strcmp(p, "q") == 0) {
            break;
        }

        clear();      //清屏
        move(3, 14);
        addstr(p);    //添加输出到屏幕
    }

    free(p);          //释放输出字符串
    p = NULL;
    tip_end(_TOTLE, _LENGTH, &tip);//结束结构体
    endwin();         //结束窗口
    return ;
}

//宏定义添加显示字符串
#define _addcol addstr("│");
#define _addrow addstr("─");
#define _mid 10

//窗口边框绘制
void winbd(int x, int y) {

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
