/*************************************************************************
	> File Name: common.c
	> Author: Allen Lee 
	> Mail: trainlee1024@gmail.com
	> Created Time: Mon Jul 13 20:35:52 2020
 ************************************************************************/



#include "common.h"


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

//获取窗口尺寸函数
int get_winsize(int flag) {
    struct winsize size;
    int ret = 0;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);

    /*
     *flag == COL 返回高度
     *flag == ROW 返回宽幅
     *否则返回（传入错误flag） -1
     */
    switch (flag) {
        case COL: {
            ret = size.ws_col;
            break;
        }
        case ROW: {
            ret = size.ws_row;
            break;
        }
        default : {
            ret = -1;
        }
    }
    return ret; 
}

//添加有颜色的输出
void sta_color(int x, int y, int foreground, int background, char *p) {
	init_pair(1, foreground, background); //初始化颜色，前景、背景
    //开始有颜色输出
	attron(COLOR_PAIR(1));
	move(x, y);
	addstr(p);
    //结束有颜色输出
	attroff(COLOR_PAIR(1));/*close color show*/
}

