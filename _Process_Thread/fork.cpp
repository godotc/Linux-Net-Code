
#include <iostream>
#include <unistd.h>

using std::cout, std::endl;

int main()
{
    std::cout << "main process Pid=" << getpid() << std::endl;
    auto pid = fork();

    if (pid == 0)
    {
        cout << "Child Pid=" << getpid() << endl;
        cout << "Father Pid=" << getppid() << endl;
    }
    else if (pid > 0)
    {
        cout << "Father Pid=" << getpid() << endl;
    }

    return 0;
}