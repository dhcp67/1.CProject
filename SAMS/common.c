/*************************************************************************
	> File Name: common.c
	> Author: Allen Lee 
	> Mail: trainlee1024@gmail.com
	> Created Time: Mon Jul 13 20:35:52 2020
 ************************************************************************/



#include "common.h"


//获取窗口尺寸函数
int get_winsize(int flag) {
    struct winsize size;
    int ret;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);

    /*
     *flag == COL 返回列数
     *flag == ROW 返回行数
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
