//模拟实现 ls -l 指令
//-rw-r--r-- 1 gloria gloria 722 May  5 17:32 func_open.c

#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<string.h>
int main(int argc, char** argv) {

    if (argc < 2) {
        printf("%s filename\n", argv[0]);
        return -1;
    }

    //
    struct stat st;
    if (-1 == stat(argv[1], &st)) {
        perror("stat");
        return -1;
    }

    //获取文件类型和文件权限
    char perms[11] = { 0 };

    //文件类型
    switch (st.st_mode & __S_IFMT) {
    case __S_IFLNK:
        perms[0] = '1';
        break;
    case __S_IFDIR:
        perms[0] = 'd';
        break;
    case __S_IFREG:
        perms[0] = '-';
        break;
    case __S_IFBLK:
        perms[0] = 'b';
        break;
    case __S_IFSOCK:
        perms[0] = 's';
        break;
    case __S_IFIFO:
        perms[0] = 'p';
        break;
    default:
        perms[0] = '?';
        break;
    }


    //文件访问权限

        //文件所有者
    perms[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';

    //文件所在组
    perms[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';

    //其他人
    perms[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
    perms[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';

    //硬链接数
    int linkNum = st.st_nlink;

    //文件所有者
    char* fileUser = getpwuid(st.st_uid)->pw_name;

    //文件所在组
    char* fileGroup = getgrgid(st.st_gid)->gr_name;

    //文件大小
    long int fileSize = st.st_size;

    //修改时间
    char* modifyTime = ctime(&st.st_mtime);


    char mtime[512] = { 0 };
    //去掉 ctime 后面的换行
    strncpy(mtime, modifyTime, strlen(modifyTime) - 1);


    char buf[1024];

    sprintf(buf,
        "%s %d %s %s %ld %s %s",
        perms, linkNum, fileUser, fileGroup, fileSize, mtime, argv[1]);

    printf("%s\n", buf);

    return 0;

}