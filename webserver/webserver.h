// encode UTF-8

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <unordered_map>
#include <fcntl.h>       // fcntl()
#include <unistd.h>      // close()
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../epoll/epoller.h"
#include "../timer/timer.h"
#include "../threadpool/threadpool.h"
#include "../http/HTTPconnection.h"

class WebServer {
public:
    WebServer(int port,int trigMode,int timeoutMS,bool optLinger,int threadNum);
    ~WebServer();

    void Start(); //一切的开始

private:
    
    bool initSocket_(); //对服务端的socket进行设置，最后可以得到listenFd
    
    void initEventMode_(int trigMode); //事件初始化

    void addClientConnection(int fd, sockaddr_in addr); //添加一个HTTP连接
    void closeConn_(HTTPconnection* client);            //关闭一个HTTP连接

  
    void handleListen_(); //处理监听事件
    void handleWrite_(HTTPconnection* client); //处理写事件
    void handleRead_(HTTPconnection* client); //处理读事件

    void onRead_(HTTPconnection* client); //添加读事件
    void onWrite_(HTTPconnection* client); //添加写事件
    void onProcess_(HTTPconnection* client); //添加边缘触发事件

    void sendError_(int fd, const char* info);
    void extentTime_(HTTPconnection* client);

    static const int MAX_FD = 65536;
    static int setFdNonblock(int fd);

    int port_;//端口号
    int timeoutMS_;  /* 毫秒MS,定时器的默认过期时间 */
    bool isClose_;//是否关闭
    int listenFd_;//监听描述符
    bool openLinger_;//是否优雅的关闭
    char* srcDir_;//需要获取的路径
    
    uint32_t listenEvent_;
    uint32_t connectionEvent_;
   
    std::unique_ptr<TimerManager>timer_; //计时器
    std::unique_ptr<ThreadPool> threadpool_; //线程池
    std::unique_ptr<Epoller> epoller_; //epoll
    std::unordered_map<int, HTTPconnection> users_; //连接
};


#endif //WEBSERVER_H