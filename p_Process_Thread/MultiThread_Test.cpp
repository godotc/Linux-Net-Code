#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

void hello()
{
    std::cout << "hello world" << endl;
}

void world(string name)
{
    cout << name << endl;
}

void test1()
{
    std::thread t(hello);
    t.join();

    std::thread t2([] { std::cout << "hello lamda" << endl; });
    t2.join();

    std::thread t3(world, "std::thread Transfer paramter");
    t3.join();
}

void test2()
{
}

int main()
{
    test1();
}