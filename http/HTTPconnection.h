// encode UTF-8


#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H

#include<arpa/inet.h> //sockaddr_in
#include<sys/uio.h> //readv/writev
#include<iostream>
#include<sys/types.h>
#include<assert.h>

#include "../buff/buffer.h"
#include "HTTPrequest.h"
#include "HTTPresponse.h"

class HTTPconnection{
public:
    HTTPconnection();
    ~HTTPconnection();

    void initHTTPConn(int socketFd,const sockaddr_in& addr); //初始化HTTP连接

    //每个连接中定义的对缓冲区的读写接口
    ssize_t readBuffer(int* saveErrno);
    ssize_t writeBuffer(int* saveErrno);

    void closeHTTPConn();//关闭HTTP连接的接口
    
    bool handleHTTPConn();//定义处理该HTTP连接的接口，主要分为request的解析和response的生成

    //其他方法
    const char* getIP() const; //获取IP
    int getPort() const; //获取端口
    int getFd() const; //获取文件描述符
    sockaddr_in getAddr() const; //获取完整的地址

    int writeBytes() //获取已经写入的数据长度
    {
        return iov_[1].iov_len+iov_[0].iov_len;
    }

    bool isKeepAlive() const //获取这个HTTP连接KeepAlive的状态
    {
        return request_.isKeepAlive();
    }

    static bool isET;
    static const char* srcDir; //当前目录的地址
    static std::atomic<int>userCount; //统计HTTP的连接个数

private:
    int fd_;                  //HTTP连接对应的描述符
    struct sockaddr_in addr_; //保存地址
    bool isClose_;            //标记是否关闭连接

    int iovCnt_;
    struct iovec iov_[2]; //写

    Buffer readBuffer_;       //读缓冲区
    Buffer writeBuffer_;      //写缓冲区

    HTTPrequest request_;     //解析请求
    HTTPresponse response_;   //响应请求

};

#endif //HTTP_CONNECTION_H