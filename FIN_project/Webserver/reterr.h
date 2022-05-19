#ifndef RETERR_H
#define RETERR_H

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


static void reterr(int ret, const char* str, bool isex = true) {

    if (-1 == ret) {
        perror(str);
        if (isex) exit(-1);
    }
}

static void _reterr(int num, int ret, const char* str) {
    if (num == ret) {
        perror(str);
        exit(num);
    }
}


#endif