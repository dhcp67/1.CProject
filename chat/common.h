/*************************************************************************
	> File Name: common.h
	> Author: lcnh 
	> Mail: 318082789@qq.com
	> Created Time: Sun Jun  2 10:32:52 2019
 ************************************************************************/

#ifndef DEBUG
#define DBG(a, b) printf(a,b);
#else 
#define DBG(a,b) 
#define
#endif

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <wait.h>
#include <signal.h>

