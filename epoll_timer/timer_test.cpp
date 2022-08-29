#include <cstdio>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

int main()
{
    alarm(1);
    long long i = 0;
    while (1)
    {
        i++;
        cerr << i << endl;
    }

    return 0;
}