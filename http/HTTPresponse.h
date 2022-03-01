// encode UTF-8


#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <unordered_map>
#include <fcntl.h>  //open
#include <unistd.h> //close
#include <sys/stat.h> //stat
#include <sys/mman.h> //mmap,munmap
#include <assert.h>

#include "../buff/buffer.h"

class HTTPresponse
{
public:
    HTTPresponse();
    ~HTTPresponse();

    void init(const std::string& srcDir,std::string& path,bool isKeepAlive=false,int code=-1);//初始化
    void makeResponse(Buffer& buffer); //生成响应报文的主函数
    void unmapFile_(); //共享内存的扫尾
    char* file();//文件信息
    size_t fileLen() const;//文件信息
    void errorContent(Buffer& buffer,std::string message); //文件打不开时返回相应的错误信息
    int code() const {return code_;} //返回状态码


private:
    void addStateLine_(Buffer& buffer); //生成请求行
    void addResponseHeader_(Buffer& buffer); //生成请求头
    void addResponseContent_(Buffer& buffer); //生成请求数据体

    void errorHTML_();//获取错误码
    std::string getFileType_(); //获取文件类型信息

    int code_; //HTTP的状态
    bool isKeepAlive_; //是否处于KeepAlive

    std::string path_; //解析出的路径
    std::string srcDir_; //根目录

    char* mmFile_;//共享内存的映射
    struct  stat mmFileStat_;//保存文件信息

    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE; //后缀名到文件类型的映射关系
    static const std::unordered_map<int, std::string> CODE_STATUS; //状态码到相应状态(字符串类型)的映射
    static const std::unordered_map<int, std::string> CODE_PATH; //错误码到响应文件路径的映射
    
};



#endif //HTTP_RESPONSE_H