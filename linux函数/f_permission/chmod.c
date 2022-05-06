#include<sys/stat.h>
// #include <sys/stat.h>

// int chmod(const char* pathname, mode_t mode);
// int fchmod(int fd, mode_t mode);

// mode:8进制的数字 0777

int main() {

    chmod("a.out", 0777);

    return 0;
}