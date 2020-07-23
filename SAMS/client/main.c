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
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

#define COL 1
#define ROW 2
#define MID (get_winsize(ROW))

void color_print(int x, int y, int foreground, int background, char *p); //添加有颜色的输出
int get_winsize(int flag); //获取窗口尺寸函数


//页面框架
void gui() {
    box(stdscr,'|','-');    //'|'竖边框,'-'横边框
    char *stu_str = "学生信息管理系统";     //标题
    int mid = (get_winsize(COL) - strlen(stu_str)) / 2 - 1; //中点
    move(0, mid);   //移动
    addstr("学生信息管理系统"); //添加字符串
}

//菜单二维数组
char menu_str[16][45] = {
    "1.  输入记录                  ",
    "2.  输出学生信息              ",
    "3.  按学号搜索                ",
    "4.  按名字搜素                ",
    "5.  计算每个学生的总成绩和平均",
    "6.  按分数降序排              ",
    "7.  按分数升序排序            ",
    "8.  按学号升序排              ",
    "9.  按字典序排序              ",
    "10. 按学号删除                ",
    "11. 按姓名删除                ",
    "12. 删除全部学生              ",
    "13. 统计分析                  ",
    "14. 算每门课程的总分和平均    ",
    "15. 保存到服务器              ",
    "0. 退出                       ",
};


//菜单
void menu() {
    int len = sizeof(menu_str[1]);      //计算长度
    for (int i = 0; i < 16; i++) {      //循环遍历菜单字符串
        move(i + 1, 3);                 //移动
        addstr(menu_str[i]);            //添加
    }
    int right = get_winsize(COL) - 1;   //右边框
    for (int i = 1; i < right; i++) {   //循环输出操作
        move(17, i);                    //移动
        addch('-');                     //添加'-'
    }
    return ;
}

//输出函数
int main() {

    setlocale(LC_ALL,"");                //设置编码
    WINDOW *wind = initscr();            //初始化一个窗口

    char *p = (char *)malloc(sizeof(char) * 100);   //一个字符串用来输出
    
    while (true) {                                  //循环输出操作
        gui();
        menu();

        
        refresh();
        //绘制有颜色的字符串
		//color_print(2, 1, COLOR_RED, COLOR_BLACK, "hello");
        
        echo();     //输入是否可见
        move(19, 14);
        getstr(p);    //输入这个字符串
        move(20, 14);
        addstr("");    //添加输出到屏幕

        //输入为q时退出
        if (strcmp(p, "q") == 0) {
            break;
        }

    }

    free(p);          //释放输出字符串
    p = NULL;
    endwin();         //结束窗口
    return 0;
}


//添加有颜色的输出
void color_print(int x, int y, int foreground, int background, char *p) {	
    if(start_color()==OK) {                     //判断终端是否支持有颜色输出
	    init_pair(1, foreground, background);   //初始化颜色，前景、背景

	    attron(COLOR_PAIR(1));                  //开始有颜色输出
	    move(x, y);                             //移动到x,y
	    addstr(p);                              //绘制
	    attroff(COLOR_PAIR(1));                 //结束有颜色输出
    } else {
	    move(x, y);                             //移动到x,y
	    addstr(p);                              //绘制
    }
}

//获取窗口尺寸函数
int get_winsize(int flag) {
    struct winsize size;
    int ret = 0;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);   //初始化窗口尺寸结构体

    switch (flag) {
        case COL: { //flag == COL 返回高度
            ret = size.ws_col;
            break;
        }
        case ROW: { //flag == ROW 返回宽幅
            ret = size.ws_row;
            break;
        }
        default : { //否则返回-1(传入错误flag)
            ret = -1;
        }
    }
    return ret; 
}
