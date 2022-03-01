// encode UTF-8


#include <unistd.h>
#include "webserver/webserver.h"

int main(int argc, char* argv[]) {
    if( argc <= 1 ) {
        printf( "请输入: %s 端口号\n", basename(argv[0]));
        return 1;
    }
    //将端口号由字符串转整数
    int port = atoi( argv[1] );
    WebServer server(port, 3, 60000, false, 4);            
    server.Start();
} 