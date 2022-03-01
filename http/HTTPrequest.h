// encode UTF-8


#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>

#include "../buff/buffer.h"

class HTTPrequest
{
public:
    enum PARSE_STATE{
        REQUEST_LINE,//请求行
        HEADERS,//请求头
        BODY,//请求体
        FINISH,//完成
    };

    enum HTTP_CODE {
        NO_REQUEST = 0,//请求不完整，需要继续读取客户数据
        GET_REQUEST,//表示获得了一个完成的客户请求
        BAD_REQUEST,//表示客户请求语法错误
        NO_RESOURSE,//表示服务器没有资源
        FORBIDDENT_REQUEST,//表示客户对资源没有足够的访问权限
        FILE_REQUEST,//文件请求,获取文件成功
        INTERNAL_ERROR,//表示服务器内部错误
        CLOSED_CONNECTION,//表示客户端已经关闭连接了
    };

    HTTPrequest() {init();};
    ~HTTPrequest()=default;

    void init();//初始化
    bool parse(Buffer& buff); //解析HTTP请求

    //获取HTTP信息
    std::string path() const;//获取路径
    std::string& path();//获取路径
    std::string method() const;//获取方式
    std::string version() const;//获取版本
    std::string getPost(const std::string& key) const;//获取指定的post信息
    std::string getPost(const char* key) const;//获取指定的post信息

    bool isKeepAlive() const;//获取是否KeepAlive

private:
    bool parseRequestLine_(const std::string& line);//解析请求行
    void parseRequestHeader_(const std::string& line); //解析请求头
    void parseDataBody_(const std::string& line); //解析数据体


    void parsePath_();//解析路径信息
    void parsePost_();//解析post报文

    static int convertHex(char ch);//转换为Hex格式的

    PARSE_STATE state_;//状态机
    std::string method_,path_,version_,body_;//HTTP方式、路径、版本、数据体
    std::unordered_map<std::string,std::string>header_;//请求头解析的信息
    std::unordered_map<std::string,std::string>post_;//post解析的信息

    static const std::unordered_set<std::string>DEFAULT_HTML;//默认网页名称
   


};





#endif  //HTTP_REQUEST_H