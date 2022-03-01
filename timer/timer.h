// encode UTF-8

#ifndef TIMER_H
#define TIMER_H

#include<queue>
#include<deque>
#include<unordered_map>
#include<ctime>
#include<chrono>
#include<functional>
#include<memory>

#include "../http/HTTPconnection.h"

typedef std::function<void()> TimeoutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;
//typedef std::unique_ptr<HTTPconnection> HTTPconnection_Ptr;

class TimerNode{
public:
    int id;             //记录连接的文件描述符，用来标记定时器
    TimeStamp expire;   //设置过期时间
    TimeoutCallBack cb; //设置一个回调函数用来方便删除定时器时将对应的HTTP连接关闭

    //需要的功能可以自己设定
    bool operator<(const TimerNode& t)
    {
        return expire<t.expire;
    }
};

class TimerManager{
    typedef std::shared_ptr<TimerNode> SP_TimerNode;
public:
    TimerManager() {heap_.reserve(64);}
    ~TimerManager() {clear();}
    void addTimer(int id,int timeout,const TimeoutCallBack& cb);//设置定时器 
    void handle_expired_event();//处理过期的定时器
    int getNextHandle();//下一次处理过期定时器的时间
    void update(int id,int timeout);//更新定时器
    void work(int id);//删除指定id的节点，并且用指针触发处理函数
    void pop();//取出堆顶的定时器
    void clear();//清除定时器

private:
    void del_(size_t i); // 删除指定的定时器
    void siftup_(size_t i); // 向上调整
    bool siftdown_(size_t index,size_t n); // 向下调整
    void swapNode_(size_t i,size_t j); // 交换两个节点的位置

    std::vector<TimerNode>heap_;
    std::unordered_map<int,size_t>ref_;// 映射一个fd对应的定时器在heap_中的位置
};

#endif //TIMER_H