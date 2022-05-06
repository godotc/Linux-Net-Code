/*
    #include <sys/types.h>
    #include <dirent.h>

    DIR *opendir(const char *name);
*/

// #include <dirent.h>

// struct dirent* readdir(DIR* dirp);

// #include <sys/types.h>
// #include <dirent.h>

// DIR* opendir(const char* name);
// DIR* fdopendir(int fd);



#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int getFileNum(const char* path);

//读取目录下普通文件的个数
int main(int argc, char** argv) {

    if (argc < 2) {
        printf("%s path\n", argv[0]);
        return -1;
    }

    int num = getFileNum(argv[1]);


    printf("普通文件 个 数为:%d\n",num);

    return 0;
}



//用于获取目录下所有普通文件的个数
int getFileNum(const char* path) {
    //1.打开目录
    DIR* dir = opendir(path);
    if (NULL == dir) {
        perror("opendir");
        _exit(0);
    }


    struct dirent* ptr;
    //记录普通文件的个数
    int total = 0;

    while (ptr = readdir(dir))
    {
        //获取名称
        char* dname = ptr->d_name;


        //忽略掉 ./ 和 ../
        if (strcmp(dname, ".") == 0 || strcmp(dname, "..") == 0) {
            continue;
        }

        //判断是普通文件还是目录

        if (ptr->d_type == 4) {
            //目录 继续读取
            char newpath[256];
            sprintf(newpath, "%s/%s", path, dname);
            total += getFileNum(newpath);
        }

        if (ptr->d_type == 8) {
            //文件
            total++;
        }
    }

    closedir(dir);

    return total;
}