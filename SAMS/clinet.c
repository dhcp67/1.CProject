/*************************************************************************
	> File Name: clinet.c
	> Author: Allen Lee 
	> Mail: trainlee1024@gmail.com
	> Created Time: Sun Jul 19 18:27:18 2020
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//debug宏
#ifdef DEBUG
#define DEBUG_PRINT(a, ...) printf(a, ##__VA_ARGS__);   //debug输出
#else
#define DEBUG_PRINT(s)
#endif

#define MAX_LEN 20
#define CONF_FILE "./conf"
#define TRUE    1                       // 正确宏
#define FALSE   0                       // 错误宏
#define LOG_FILE        "clinet.log"    // 日志文件名
#define STUDENT_FILE    "student.txt"   // 学生数据文件
#define LEN sizeof(struct student)

//学生结构体
typedef struct student STU;
//学生结构体

typedef struct student {
        long id;                                // 每个学生的学号
        char name[MAX_LEN];                     // 每个学生的姓名
        float score[MAX_LEN];                   // 每个学生COURSE_NUM门功课的成绩
        float sum;                              // 每个学生的总成绩
        float aver;                             // 每个学生的平均成绩
        struct student *next;
}STU;

//学生管理结构体
typedef struct student_manage STU_M;

//学生管理结构体
typedef struct student_manage {
    int stu_num;                        //学生数量
    int course_num;                     //课程数量
    char **course_name;                 //课程名
    STU *head;                          //学生头结点
} STU_M;

int send_stu_data(STU_M *, const int);          //发送数据给服务器
int socket_connect(char *, int);                //连接函数
int get_conf_value(char *, char *, char *);     //获取配置文件
int str_to_int(char *str);                      //字符串转换整型
int get_stu_msg(STU_M *);                       //输入学生信息
int on_id_del(STU_M *);                         //按学号删除
int on_name_del(STU_M *);                       //按姓删除
int all_del(STU_M *);                           //删除全部学生
int get_conf_value(char *f, char *, char *);    //获取学生数和课程数
int save_to_server(STU_M *, int);               //上传学生信息
char *get_date_time();                          //获取日期时间的函数
char *menu(char *);                             //创建菜单
void over(int);                                 //让服务器断开连接
void writ_log_file(int , const char *);         //写入日志文件
void print_msg(STU_M *);                        //打印信息函数
void sum_avg_every_course(STU_M *);             //计算每门课程的总分和平均分
void sum_avg_every_student(STU_M *);            //计算每个学生的总分和平均分
void creat_student(STU_M *);                    //创建链表并录入信息
void search_by_Num(STU_M *);                    //按学号查询学生排名及其考试成绩
void search_by_Name(STU_M *);                   //按姓名查询学生排名及其考试成绩
void statistic_analysis(STU_M *);               //统计输出
void end_student(STU_M *);                      //结束学生管理
STU *new_node();                                //创建新节点
STU_M *sort_by_score(STU_M *);                  //按每个学生的总分由高到低排出名次表
STU_M *sort_by_score_reverse(STU_M *);          //按每个学生的总分由低到高排出名次表
STU_M *creat(STU_M *);                          //创建学生链表
STU_M *sort_by_num(STU_M *);                    //按学号由小到大排出成绩表
STU_M *sort_by_name(STU_M *);                   //按姓名的字典顺序排出成绩表
STU_M *get_server_student();                    //从服务器获取学生数据




int main() {
    int _con = TRUE;     //con用来做循环判断
    int sockfd; //定义一个套接字变量
    char *ip = (char *)malloc(sizeof(char) * MAX_LEN); //定义一个ip并申请空间
    char *port = (char *)malloc(sizeof(char) * MAX_LEN); //定义一个ip并申请空间
	get_conf_value(CONF_FILE, "To", ip);    //获取ip地址
    get_conf_value(CONF_FILE, "ServerPort", port);//获取端口
    sockfd = socket_connect(ip, atoi(port)); 
    if (sockfd < 0) {
        printf("连接失败\n");
        exit(-1);
    }

    char *flag = (char *)malloc(sizeof(char) * MAX_LEN);    //定义一个输入判断标志并并申请空间
    STU_M *stu;
    stu = get_server_student(sockfd);                            //从服务器获取学生数据

    //循环操作
    while(_con) {
        flag = menu(flag);
        switch (str_to_int(flag)) {
            case 1:
                system("clear");  //清屏
                get_stu_msg(stu);
                break;

            case 2:
                system("clear");  //清屏
                print_msg(stu);
                break;
            case 3:
                system("clear");  //清屏
                printf("\n\n\n");
                printf("\t\t\t******************************************************************************\n");
                printf("\t\t\t请输入你想要搜索的号码:\n");
                search_by_Num(stu);
                break;
            case 4:
                system("clear");  //清屏
                printf("\n\n\n");
                printf("\t\t\t******************************************************************************\n");
                printf("\t\t\t请输入你想要搜索的姓:\n");
                search_by_Name(stu);
                break;
            case 5:
                system("clear");  //清屏
                sum_avg_every_student(stu);
                break;
            case 6:
                system("clear");  //清屏
                printf("\n\n\n");
                printf("\t\t\t按分数升序排序:\n");
                stu = sort_by_score(stu);
                print_msg(stu);
                break;
            case 7:
                system("clear");  //清屏
                printf("\n\n\n");
                printf("\t\t\t按分数升序排序:\n");
                stu = sort_by_score_reverse(stu);
                print_msg(stu);
                break;
            case 8:
                system("clear");  //清屏
                printf("\n\n\n");
                printf("\t\t\t按学号升序排:\n");
                stu = sort_by_num(stu);
                print_msg(stu);
                break;
            case 9:
                system("clear");  //清屏
                printf("\n\n\n");
                printf("\t\t\t按名字的字典序排:\n");
                stu = sort_by_name(stu);
                print_msg(stu);
                break;
            case 10:
                on_id_del(stu);
                break;
            case 11:
                on_name_del(stu);
                break;
            case 12:
                system("clear");
                all_del(stu);
                break;
            case 13:
                system("clear");  //清屏
                statistic_analysis(stu);
                break;
            case 14:
                system("clear");  //清屏
                sum_avg_every_course(stu);
                break;
            case 15:
                system("clear");  //清屏
                save_to_server(stu, sockfd);
                break;
            case 0:
                system("clear");  //清屏
                over(sockfd);
                printf("\t\t\t程序结!\n");
                _con = FALSE;
            default:
                system("clear");  //清屏
                printf("\n\n\n");
                printf("\t\t\t******************************************************************************\n");
                printf("\t\t\t输入错误!\n");
                printf("\t\t\t******************************************************************************\n");

        }

    }
    free(flag);//释放判断标志
    flag = NULL;//赋空值,避免野指针
    end_student(stu);//

    free(ip);
    free(port);
    return 0;
}

//从服务器获取学生数据
STU_M *get_server_student(int sockfd) {
    STU_M *stu = (STU_M *)malloc(sizeof(STU_M));    //为学生管理结构体开辟空间
    stu->stu_num = 0;                               //学生数为赋值0
    stu->course_num = 0;                            //课程数赋值为0
    int len = sizeof(STU_M);                        //len为STU大小
    int rec_v;                                      //定义一个返回值
    rec_v = recv(sockfd, stu, len, 0);              //发送管理结构体
    if (rec_v <= 0) {                                //如果发送失败
        writ_log_file(FALSE, "接收学生管理结构");   
    }
    
    //为课程名申请空间
    stu->course_name = (char **)malloc(sizeof(char*) * (stu->course_num));  //申请一维地址空间
    for (int i = 0; i < stu->course_num; ++i) {                             //循环申请空间
        stu->course_name[i] = (char *)malloc(sizeof(char) * MAX_LEN);       //申请空间
    }

    //课程名接收
    for (int i = 0; i < stu->course_num; ++i) {                             //循环接收课程名
        rec_v = recv(sockfd, stu->course_name[i], MAX_LEN, 0);              //接收课程名
        if (rec_v < 0) {
            writ_log_file(FALSE, "接收课程名");                             //写日志
        }
    }

    len = sizeof(STU);                                                      //len为学生节点大小
	stu = creat(stu);                                                       //创建学生链表
    STU *node = stu->head;                                                  //创建一个学生节点
    STU *tmp = node->next;                                                  //创建一个临时节点


    //接收学生信息
    for (int i = 0; i < stu->stu_num; i++) {
        if (i) {
            node = node->next;                                              //节点等于下一个节点
            tmp = tmp->next;                                                //临时节点等于下一个节点
        }
        
        rec_v = recv(sockfd, node, len, 0);                                 //接收学生信息
        if (rec_v < 0) {
            writ_log_file(FALSE, "接收学生结点");
        }
        
        node->next = tmp;                                                   //学生节点下一个节点等于临时节点
    }
    return stu;
}

//上传学生信息
int save_to_server(STU_M *stu, int sockfd) {
    int ret;                                        //定义一个返回值
    int flag = 1;                                   //定义一个标志
    ret = send(sockfd, &flag, sizeof(flag), 0);     //发送标志
    if (ret <= 0) {                                 //如果返回值小于等于0，发送失败
        writ_log_file(FALSE, "发送flag失败");       //写日志
    } else {
        flag = send_stu_data(stu, sockfd);          //发送学生数据
        if (flag <= 0) {
            writ_log_file(FALSE, "上传数据失败");   //写日志
        }
    }
    if (flag > 0) {
        writ_log_file(TRUE, "成功上传数据");        //写日志
        DEBUG_PRINT("成功上传数据\n");
    }
    return ret;
}

//创建新节点
STU *new_node() {
    STU *node = (STU *)malloc(sizeof(STU));     //申请节点空间
    node->next = NULL;                          //下一个节点指向NULL
    return node;
}

//连接函数
int socket_connect(char *host, int port) {
    int sockfd;     //定义一个套接字
    struct sockaddr_in server; //定义一个地址端口结构体

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //创建一个套接字
        return -1;
    }

    server.sin_family = AF_INET;//绑定为ipv4
    server.sin_port = htons(port);//设置访问端口
    server.sin_addr.s_addr = inet_addr(host); //设置访问ip地址
    if ((connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)) {//连接失败
        //perror("connect()");
        return -1;//返回-1
    }
    return sockfd; //返回套接字
}

//获取日期时间的函数，返回一个字符串指针，用完释放
char *get_date_time() {
    char *str = malloc(sizeof(char) * 25);                      //申请一个字符串空间
    memset(str, 0, sizeof(char) * 25);                          //全置为0
    time_t timep;                                                                       //定义一个记录时间戳的变量
    struct tm *p;                                                                       //定义一个时间结构体指针
    time(&timep);                                                                       //获取时间戳
    p = gmtime(&timep);                                                         //获得时间结构体地址

    int year = 1900 + p->tm_year;                                       //获取当前年份,从1900开始，所以要加1900
    int mon = 1 + p->tm_mon;                                            //获取当前月份,范围是0-11,所以要加1
    int day = p->tm_mday;                                                       //获取当前月份日数,范围是1-31
    int hour = 8 + p->tm_hour;                                          //获取当前时,这里获取西方的时间,刚好相差八个小时
    int min = p->tm_min;                                                        //获取当前分
    int sec = p->tm_sec;                                                        //获取当前秒

    sprintf(str, "%d-%02d-%02d-%02d-%02d-%02d", year, mon, day, hour, min, sec); //格式化到字符串

    return str;

}

//日志函数
void writ_log_file(int flag, const char *str) {
    FILE *fp = fopen(LOG_FILE, "a");                    //打开日志文件
    char *d_t = get_date_time();                        //获取日期时间字符串
    char *tmpbool = (flag ? "true" : "false");          //判断是错误还是正确日志
    fprintf(fp, "%s\t%d\t%s\t%s\t%s\n", __FILE__, __LINE__, tmpbool, d_t, str);     //输出到日志文件
    free(d_t);                                          //释放返回的时间字符串空间
    fclose(fp);                                         //关闭文件描述符
}


//结束学生管理
void end_student(STU_M *stu) {
    STU *p, *tmp;                                           //定义一个指针和临时指针
    p = stu->head;                                          //临时指针指学生向头结点
    stu->head = NULL;                                       //学生管理结构体的头结点指向NULL，避免野指针

    //循环释放
    while(p) {                                              //指针不为空时循环
        tmp = p;                                            //临时指针指向p
        p = p->next;                                        //p指向下一个结点
        free(tmp);                                          //释放临时指针
    }

    p = NULL, tmp = NULL;                                   //p和临时指针支持NULL
    //循环释放课程名
    for (int i = 0; i < stu->course_num; i++) {             //遍历所有课程名
        free(stu->course_name[i]);                          //释放课程名内存
    }
    free(stu->course_name);                                 //释放一级指针
    writ_log_file(TRUE, "退出系统");                        //打印日志
}

//输出学生信息函数
void print_msg(STU_M *stu) {
    printf("学号\t姓名");                   //输出表头
    for (int i = 0; i < stu->course_num; i++) {             //循环遍历课程名
        printf("\t%s", stu->course_name[i]);              //输出课程名
    }
    printf("\t%s\t%s\n", "总分", "平均分");
    STU *tmp = stu->head;
    //while (tmp) {
    for (int i = 0; i < stu->stu_num; i++) {
        printf("%ld\t%s", tmp->id, tmp->name);
        //循环遍历分数
        for (int j = 0; j < stu->course_num; j++) {
            printf("\t%.2lf", tmp->score[j]);
        }
        printf("\t%.2lf\t%.2f\n", tmp->sum, tmp->aver);   //输出总分和平均分
        tmp = tmp->next;
    }
    return ;
}

//字符串转换为整数
int str_to_int(char *str) {
    int ret = 0, tmp = 1;
    int len = strlen(str);
    for (int i = len - 1; i >= 0; i--) {        //逆序遍历字符串
        if (str[i] >= '0' && str[i] <= '9') {   //如果字符是0-9
            ret += tmp * (str[i] - '0');        //计算值
        } else {
            ret = -1;
            break;
        }
        tmp *= 10;
    }
    return ret;
}

// 创建菜单
char *menu(char *str) {
        system("echo \'title 学生成绩管理系统 By lee\'");
        printf("\n\n");
        printf("\t\t\t                         学生成绩管理系统\n");
        printf("\t\t\t******************************************************************************\n");
        printf("\t\t\t*                学生成绩管理                                                *\n");
        printf("\t\t\t*                1.  输入记录                                                *\n");
        printf("\t\t\t*                2.  输出学生信息                                            *\n");
        printf("\t\t\t*                3.  按学号搜索                                              *\n");
        printf("\t\t\t*                4.  按名字搜素                                              *\n");
        printf("\t\t\t*                5.  计算每个学生的总成绩和平均                              *\n");
        printf("\t\t\t*                6.  按分数降序排                                            *\n");
        printf("\t\t\t*                7.  按分数升序排序                                          *\n");
        printf("\t\t\t*                8.  按学号升序排                                            *\n");
        printf("\t\t\t*                9.  按字典序排序                                            *\n");
        printf("\t\t\t*                10. 按学号删除                                              *\n");
        printf("\t\t\t*                11. 按姓名删除                                              *\n");
        printf("\t\t\t*                12. 删除全部学生                                             *\n");
        printf("\t\t\t*                13. 统计分析                                                *\n");
        printf("\t\t\t*                14. 算每门课程的总分和平均                                  *\n");
        printf("\t\t\t*                15. 保存到服务器                                                *\n");
        printf("\t\t\t*                0. 退出                                                 *\n");
        printf("\t\t\t******************************************************************************\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\t\t\t*****************请输入你的选择:");
        scanf("%s", str);
        return str;

}

// 创建学生信息链表
void creat_student(STU_M *stu) {
    STU *tmp;                                       //定义一个临时学生结点
    STU *end = stu->head;                           //定义一个尾指针

    while (end->next != NULL) end = end->next;      //循环找到尾指针
    //循环添加信息
    for (int i = 0; i < stu->stu_num; i++) {
        tmp = (STU *)malloc(sizeof(STU *));         //给临时指针申请空间
        end->next = tmp;                            //赋值给临时指针的下个结点
        end = end->next;                            //尾指针移到尾部
        scanf("%ld", &(end->id));                   //输入学号
        scanf("%s", end->name);                     //输入姓名
        for (int j = 0; j < stu->course_num; j++) { //循环输入成绩
            scanf("%f", &(end->score[j]));          //输入成绩
        }
    }
        return ;
}

//创建学生链表函数
STU_M *creat(STU_M *stu) {
        STU *head;
        STU *p1, *p2;
        int n = stu->stu_num;
        int m = stu->course_num;
        // system("clear");
        for (int i = 1; i < n + 1; i++) {
                p1 = (STU *)malloc(sizeof(STU));
                p1->next = NULL;
                if (i == 1) {
                        head = p2 = p1;
                }
                else {
                        p2->next = p1;
                        p2 = p1;
                }
        }
        stu->head = head;
        return stu;
}


//计算每门课程的总分和平均分
void  sum_avg_every_course(STU_M *stu) {
        STU * head = stu->head;
    int n = stu->stu_num, m = stu->course_num;
        STU *p;
        int  j = 1;
        char ch;
        float sum;
        p = head;
        if (head != NULL) {
                printf("\t\t\t******************************************************************************\n");
                for (int i = 0; i < m; i++) {
                        p = head;
                        sum = 0.0;
                        do {
                                sum += p->score[i];
                                p = p->next;
                        } while (p != NULL);

                        printf("\t\t\tcourse %s:    sum=%.0f , aver=%.0f\n", stu->course_name[i], sum, sum / n);
                        j++;
                }
                printf("\t\t\t******************************************************************************\n");

        }
}

//计算每个学生的总分和平均分
void  sum_avg_every_student(STU_M *stu) {
    STU *node = stu->head;
    while (node) {
        float sum = 0.0;
        for (int i = 0; i < stu->course_num; i++) {
            sum += node->score[i];
        }
        node->sum = sum;
        node->aver = sum / stu->course_num;
        node = node->next;
    }
    print_msg(stu);
    return ;
}

STU_M  *sort_by_score(STU_M *stu) {
    STU *head = stu->head;
    int n = stu->stu_num;
        STU *endpt;    //控制循环比较
        STU *p;        //临时指针变量
        STU *p1, *p2;

        p1 = (STU *)malloc(LEN);
        p1->next = head;        //注意理解：我们增加一个节点，放在第一个节点的前面，主要是为了便于比较。因为第一个节点没有前驱，我们 不能交换地址
        head = p1;                 //让head指向p1节点，排序完成后，我们再把p1节点释放掉

        for (endpt = NULL; endpt != head; endpt = p) {
                for (p = p1 = head; p1->next->next != endpt; p1 = p1->next) {
                        if (p1->next->sum < p1->next->next->sum)  { //如果前面的节点键值比后面节点的键值大，则交换
                                p2 = p1->next->next;
                                p1->next->next = p2->next;
                                p2->next = p1->next;
                                p1->next = p2;
                                p = p1->next->next;
                        }
                }
        }

        p1 = head;              //把p1的信息去掉
        head = head->next;       //让head指向排序后的第一个节点
        free(p1);          //释放p1
        p1 = NULL;          //p1置为NULL，保证不产生“野指针”，即地址不确定的指针变量
    stu->head = head;
        return stu;
}

STU_M  *sort_by_score_reverse(STU_M *stu) {
    STU *head = stu->head;
    int n = stu->stu_num;
        STU *endpt;    //控制循环比较
        STU *p;        //临时指针变量
        STU *p1, *p2;

        p1 = (STU *)malloc(LEN);
        p1->next = head;        //注意理解：我们增加一个节点，放在第一个节点的前面，主要是为了便于比较。因为第一个节点没有前驱，我们 不能交换地址
        head = p1;                 //让head指向p1节点，排序完成后，我们再把p1节点释放掉

        for (endpt = NULL; endpt != head; endpt = p) {
                for (p = p1 = head; p1->next->next != endpt; p1 = p1->next) {
                        if (p1->next->sum > p1->next->next->sum)  {     //如果前面的节点键值比后面节点的键值大，则交换
                                p2 = p1->next->next;
                                p1->next->next = p2->next;
                                p2->next = p1->next;
                                p1->next = p2;
                                p = p1->next->next;
                        }
                }
        }

        p1 = head;              //把p1的信息去掉
        head = head->next;       //让head指向排序后的第一个节点
        free(p1);          //释放p1
        p1 = NULL;          //p1置为NULL，保证不产生“野指针”，即地址不确定的指针变量
    stu->head = head;
        return stu;
}

STU_M *sort_by_num(STU_M *stu) {
    STU *head = stu->head;
        STU *first;    //为原链表剩下用于直接插入排序的节点头指针
        STU *t;        //临时指针变量：插入节点
        STU *p, *q;     //临时指针变量

        first = head->next;      //原链表剩下用于直接插入排序的节点链表：可根据图12来理解
        head->next = NULL;       //只含有一个节点的链表的有序链表：可根据图11来理解

        while (first != NULL) {  //遍历剩下无序的链表
                //注意：这里for语句就是体现直接插入排序思想的地方
                for (t = first, q = head; ((q != NULL) && (q->id < t->id)); p = q, q = q->next);  //无序节点在有序链表中找插入的位置

                first = first->next; //无序链表中的节点离开，以便它插入到有序链表中

                if (q == head) {     //插在第一个节点之前
                        head = t;
                }
                else {           //p是q的前驱
                        p->next = t;
                }
                t->next = q;     //完成插入动作
                                                 //first = first->next;
        }
    stu->head = head;
        return stu;
}

STU_M  *sort_by_name(STU_M *stu) {
    int n = stu->stu_num;
    STU *head = stu->head;
        STU *endpt;    //控制循环比较
        STU *p;        //临时指针变量
        STU *p1, *p2;

        p1 = (STU *)malloc(LEN);
        p1->next = head;        //注意理解：我们增加一个节点，放在第一个节点的前面，主要是为了便于比较。因为第一个节点没有前驱，我们 不能交换地址
        head = p1;                 //让head指向p1节点，排序完成后，我们再把p1节点释放掉

        for (endpt = NULL; endpt != head; endpt = p) {
                for (p = p1 = head; p1->next->next != endpt; p1 = p1->next) {
                        if (strcmp(p1->next->name, p1->next->next->name)>0) { //如果前面的节点键值比后面节点的键值大，则交换
                                p2 = p1->next->next;
                                p1->next->next = p2->next;
                                p2->next = p1->next;
                                p1->next = p2;     //结合第4点理解
                                p = p1->next->next;   //结合第6点理解
                        }
                }
        }

        p1 = head;              //把p1的信息去掉
        head = head->next;       //让head指向排序后的第一个节点
        free(p1);          //释放p1
        p1 = NULL;          //p1置为NULL，保证不产生“野指针”，即地址不确定的指针变量
    stu->head = head;

        return stu;
}



void  search_by_Num(STU_M *stu) {
    STU *head = stu->head;
    int n = stu->stu_num;
    int m = stu->course_num;
        long num;
        int i;
        int flag = 1;
        printf("\t\t\t");
        scanf("%ld", &num);
        STU *p;
        p = head;
        if (head != NULL) {
                do {
                        if (p->id == num) {
                                printf("\t\t\t");
                                printf("%ld\t%s\t", p->id, p->name);
                                for (i = 0; i<m; i++) {
                                        printf("%.0f\t", p->score[i]);
                                }
                                printf("%.0f\t%.0f\n", p->sum, p->sum / m);
                                flag = 0;

                        }
                        p = p->next;

                } while (p != NULL);
                if (flag) {
                        printf("\t\t\t");
                        printf("Not found!\n");
                }
        }
        printf("\t\t\t******************************************************************************\n");

}

void  search_by_Name(STU_M *stu) {
    int n = stu->stu_num;
    int m = stu->course_num;
    STU *head = stu->head;
        char name[MAX_LEN];
        int i;
        int flag = 1;
        printf("\t\t\t");
        scanf("%s", name);
        STU *p;
        p = head;
        if (head != NULL) {
                do {
                        if (strcmp(name, p->name) == 0) {
                                printf("\t\t\t");
                                printf("%ld\t%s\t", p->id, p->name);
                                for (i = 0; i<m; i++) {
                                        printf("%.0f\t", p->score[i]);
                                }
                                printf("%.0f\t%.0f\n", p->sum, p->sum / m);
                                flag = 0;
                        }
                        p = p->next;

                } while (p != NULL);
                if (flag) {
                        printf("\t\t\t");
                        printf("Not found!\n");
                }
        }
        printf("\t\t\t******************************************************************************\n");

}

void  statistic_analysis(STU_M *stu) {
    int n = stu->stu_num;
    int m = stu->course_num;
        int a[6] = {0};
        STU *p;
        p = stu->head;
        double cnt = 0.0;
        for (int i = 0; i < m; i++) {
                p = stu->head; // 初始化，不然会炸
                for (int j = 0; j < 6; j++) {// 初始化
                        a[j] = 0;
                }
            cnt = 0.0;
                do {
                        if (p->score[i]<60) {
                                a[0]++, cnt++;
                        }
                        else if (p->score[i]<70) {
                                a[1]++, cnt++;
                        }
                        else if (p->score[i]<80) {
                                a[2]++, cnt++;
                        }
                        else if (p->score[i]<90) {
                                a[3]++, cnt++;
                        }
                        else if (p->score[i]<100) {
                                a[4]++, cnt++;
                        }
                        else {
                                a[5]++, cnt++;
                        }

                        p = p->next;

                } while (p != NULL);
                printf("\n\n\n");
                printf("\t\t\t******************************************************************************\n");
                printf("\t\t\t* For %s:\n", stu->course_name[i]);
                printf("\t\t\t* <60\t%d\t%.2f%%\n", a[0], 100 * a[0] / cnt);
                printf("\t\t\t* %d-%d\t%d\t%.2f%%\n", 60, 69, a[1], 100 * a[1] / cnt);
                printf("\t\t\t* %d-%d\t%d\t%.2f%%\n", 70, 79, a[2], 100 * a[2] / cnt);
                printf("\t\t\t* %d-%d\t%d\t%.2f%%\n", 80, 89, a[3], 100 * a[3] / cnt);
                printf("\t\t\t* %d-%d\t%d\t%.2f%%\n", 90, 99, a[4], 100 * a[4] / cnt);
                printf("\t\t\t* %d\t%d\t%.2f%%\n", 100, a[5],  100 * a[5] / cnt);
                printf("\t\t\t******************************************************************************\n");

        }
}


//输入学生信息
int get_stu_msg(STU_M *stu) {
        system("clear");                            //清屏
    STU *node, *tmp;                            //定义一个学生结点和临时结点
    int n = 0;

        printf("请输入要插入学生数(n<30): ");       //输入学生数
        scanf("%d", &n);                            //输入

    if (n == 0) return 0;                       //为0返回0

    if (stu->stu_num == 0) {
            printf("******************************************************************************\n");
            printf("\n");
            printf("请输入课程数(m<=6): ");
            scanf("%d", &(stu->course_num));
        stu->course_name = (char **)malloc(sizeof(char *) * stu->course_num);
        for (int i = 0; i < stu->course_num; i++) {
            stu->course_name[i] = (char *)malloc(sizeof(char) * MAX_LEN);
        }
        printf("请输入%d门课程名\n", stu->course_num);
        for (int j = 0; j < stu->course_num; j++) {
            scanf("%s", stu->course_name[j]);
        }
        stu->head = (STU *)malloc(sizeof(STU));
        node = stu->head;
        } else {
        tmp = node = stu->head;
        while (tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = (STU *)malloc(sizeof(STU));
        node = tmp->next;
    }

    stu->stu_num += n;

    node->next = NULL;
    printf("请按照以下格式输入：\n");
    printf("学号\t姓名");
    for (int i = 0; i < stu->course_num; i++) printf("\t%s", stu->course_name[i]);
    printf("\n");
    scanf("%ld", &(node->id));
    scanf("%s", node->name);
    for (int i = 0; i < stu->course_num; i++) {
        scanf("%f", &(node->score[i]));
    }

    for (int i = 1; i < n; i++) {
        node->next = (STU *)malloc(sizeof(STU));
        node = node->next;
        node->next = NULL;
        scanf("%ld", &(node->id));
        scanf("%s", node->name);
        for (int j = 0; j <stu->course_num; j++) {
            scanf("%f", &(node->score[j]));
        }
    }
    printf("\n");
    return 1;
}

//获取配置文件
int get_conf_value(char *file, char *key, char *val) {
    int ret = -1;                               //定义一个返回值
    FILE *fp = NULL;                            //定义文件指针
    char *line = NULL, *substr = NULL;          //定义行指针,子字符串指针
    size_t n = 0, len = 0;                      //定义n，长度len
    ssize_t read;                               //定义一个记录返回长度的变量

    if (key == NULL) {                          //如果key为空
        printf("error\n");                      //报错
        return ret;
    }
    fp = fopen(file, "r");                      //以只读打开文件

    if (fp == NULL) {                           //打开失败
        printf("Open CONF file error!\n");    //报错
        return ret;
    }

    while ((read = getline(&line, &n, fp))) {   //循环读取行
        substr = strstr(line, key);             //查找key是否在
        if (substr == NULL) {                   //如果没在
            continue;                           //跳过本次循环
        } else {
            len = strlen(key);                  //求取长度
            if (line[len] == '=') {             //Key后面是否为=
                strncpy(val, line + len + 1, (int)read - len - 2); //找到了配置赋值给val
                ret = 1;
                break;
            } else {
                continue;                       //否则跳过本次循环
            }
        }
    }
    if (substr == NULL) {                       //如果没找到
        printf("%s Not found in %s!\n", key, file);
        ret = -1;
    }
    fclose(fp);                                 //关闭文件
    free(line);                                 //释放line
    return 0;
}

//按学号删除函数
int on_id_del(STU_M *stu) {
    STU *head = (STU *)malloc(sizeof(STU));     //定义一个学生虚拟头结点
    head->next = stu->head;                     //虚拟头结点的下个结点等于学生实际头结点
    STU *node = head;                           //定义一个结点等于虚拟头结点
    STU *tmp;                                   //定义一个临时结点用来标记删除的结点
    int num, ret = FALSE;                       //定义输入变量和返回值
    system("clear");
    printf("请输入要删除的学号:");
    scanf("%d", &num);                          //输入

    //循环查找
    while (node->next) {
        if (node->next->id == num) {            //如果找到
            tmp = node->next;                   //临时结点标记
            if (stu->head == tmp) {             //如果删除的是头结点
                stu->head = tmp->next;          //结点后移一位
            }
            node->next = tmp->next;             //该结点越过要删除的结点
            free(tmp);                          //释放结点
            ret = TRUE;                         //标记返回值成功删除
            stu->stu_num--;
            printf("删除成功\n");
            break;                              //跳出循环
        }
        node = node->next;
    }
    free(head);                                 //释放虚拟头结点
    return ret;
}

//按姓名删除函数
int on_name_del(STU_M *stu) {
    STU *head = (STU *)malloc(sizeof(STU));     //定义一个学生虚拟头结点
    head->next = stu->head;                     //虚拟头结点的下个结点等于学生实际头结点
    STU *node = head;                           //定义一个结点等于虚拟头结点
    STU *tmp;                                   //定义一个临时结点
    int ret = FALSE;                            //定义一个返回变量
    char *str = (char *)malloc(sizeof(char) * MAX_LEN); //定义一个字符串用于输入
    system("clear");
    printf("请输入要删除的名字: ");
    scanf("%s", str);
    while (node->next) {                        //循环查找
        if (strcmp(node->name, str) == 0) {     //如果找到
            tmp = node->next;                   //临时结点标记
            if (stu->head == tmp) {             //如果删除的是头结点
                stu->head = tmp->next;          //头结点往后移动一位
            }
            node->next = tmp->next;             //结点越过要删除的结点
            free(tmp);                          //释放删除的结点
            ret = TRUE;                         //标记返回值删除成功
            stu->stu_num--;
            printf("删除成功\n");
            break;                              //跳出循环
        }
        node = node->next;
    }
    free(head);                                 //释放虚拟头结点
    free(str);                                  //释放字符串变量
    return ret;
}

//删除全部学生函数
int all_del(STU_M *stu) {
    char *dstr = (char *)malloc(sizeof(char) * MAX_LEN);    //定义一个确认字符串
    printf("确定删除全部学生信息？请输入: (1. 确定,任意键取消)\n");
    scanf("%s", dstr);                                      //输入是否删除
    int ret = str_to_int(dstr);                             //字符串转换为整数

    //删除
    if (ret == 1) {                                         //如果删除
        STU *head = stu->head;                              //定义一个头结点
        STU *tmp;                                           //定义一个临时结点
        stu->head = NULL;                                   //学生管理结构体头结点置空
        stu->stu_num = 0;                                   //学生数归零
        stu->course_num = 0;                                //课程数归零
        while(head) {                                       //遍历结点
            tmp = head;                                     //赋予临时结点
            head = tmp->next;                               //头结点指向下一个结点
            free(tmp);                                      //释放临时结点
        }
    }
    free(dstr);                                             //释放字符串
    return ret;
}

//发送数据给服务器
int send_stu_data(STU_M *stu, const int sockfd) {
    int send_v;                                             //定义一个返回值

    send_v = send(sockfd, stu, sizeof(STU_M), 0);           //发送学生管理结构体
    if (send_v <= 0) {
        writ_log_file(FALSE, "发送学生管理结构体");         //写日志
    }
	//循环发送课程名
    for (int i = 0; i < stu->course_num; ++i) {
        if (send(sockfd, stu->course_name[i], MAX_LEN, 0) <= 0) { //发送课程名
            writ_log_file(FALSE, "发送课程名");             //写日志
        }
    }

    STU *node = stu->head;                                  //定义一个节点
    int len = sizeof(STU);                                  //算出学生结构体大小
    for (int i = 0; i < stu->stu_num; i++) {                //序号发送学生数据
        if (i) {
            node = node->next;                              //如果不是第一次，节点等于下一个节点
        }
        send_v = send(sockfd, node, len, 0);                //发送节点
        if (send_v <= 0) {
            writ_log_file(FALSE, "send error\n");           //写日志
        }
    }
    return 1;
}

//给服务器发送0，关闭连接
void over(int sockfd) {
    int val = 0;
    _con = FALSE;
    send(sockfd, &val, sizeof(val), 0); //发送0
    close(sockfd);                      //关闭套接字
    return ;
}

