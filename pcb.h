#ifndef PCB
#define PCB

#include <iostream>
#include <vector>
#include <algorithm>
#include <windows.h>
using namespace std;
typedef struct PCB
{
    int pid;//进程标识符
    int runtime;//运行时间
    int priority;//优先级 优先数大的优先级高
    //就绪，运行，完成，后备，挂起
    int flag;//是否分配
    int address;//起始地址
    int size;//进程大小
}pcb,*pcbn;

#endif // PCB

