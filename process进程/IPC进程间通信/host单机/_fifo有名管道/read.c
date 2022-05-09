#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

//向管道中读数据
int main() {

    //1.打开管道        只读
    int fd = open("test_fifo", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(-1);
    }

    //4.读数据

    while (1)
    {
        char buf[BUFSIZ];
        int len = read(fd, buf, sizeof(buf));

        if (len == 0) {
            printf("写端断开链接了....\n");
            break;
        }

        printf("receive buf: %s \n", buf);
    }


    close(fd);
    return 0;
}