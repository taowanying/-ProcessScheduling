#ifndef TABLE
#define TABLE

#include <iostream>
#include <vector>
#include <algorithm>
#include <windows.h>
using namespace std;
typedef struct TABLE
{
    int pid;//分配进程的PID
    int baddress;//已分配的起点
    int size;//已分配分区大小
}stable;

typedef struct FREETABLE
{
    int baddress;//未分配的起点
    int size;//未分配分区大小
}sfreetable;

#endif // TABLE


