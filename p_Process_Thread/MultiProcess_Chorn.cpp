#include <bits/chrono.h>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

using LL = long long;
using ms = std::chrono::microseconds;

int total_sum = 0;
int total = 0;

// calculate interval summ, parallel
LL get_interval_sum(int start)
{
    start *= 20000;
    int stop = start + 20000;
    LL temp = 0;
    for (int i = start; i < stop; ++i)
    {
        temp += i;
    }
    printf("I'm %dth, temp = %lld\n", start / 20000, temp);
    total += temp;
    return temp;
}

int main()
{
    int THREAD_NUM = 5;

    pid_t pid;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    int i = 0;
    for (i = 0; i < THREAD_NUM; ++i)
    {
        pid = fork();
        if (pid == 0)
        {
            total_sum += get_interval_sum(i);
            break;
        }
    }

    if (i < THREAD_NUM)
    {
    }
    else
    {
        // 回收 recycle
        int reclaim_num = 0;

        do
        {
            // -1: all child process
            auto wpid = waitpid(-1, NULL, WNOHANG);
            
            if (wpid > 0)
            {
                reclaim_num++;
            }
        } while (reclaim_num != THREAD_NUM);

        printf("I'm parent, pid = %u, ppid =%u\n", getpid(), getppid());
        printf("Multi_Process result = %lld\n", total_sum);

        auto end = chrono::steady_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        printf("Mulit_Process takes %lld ms\n", duration_ms);
    }

    if (pid != 0)
    {
        cout << "--------------------------------------" << endl;
        start = chrono::steady_clock::now();
        int temp = 0;
        for (int i = 0; i < 5 * 20000; ++i)
        {
            temp += i;
        }
        printf("I'm parent, Single process temp= %lld\n", temp);
        auto end = chrono::steady_clock::now();
        auto duration_ms = chrono::duration_cast<ms>(end - start);
        printf("Single process, use %lld ms\n", duration_ms);
    }

    return 0;
}