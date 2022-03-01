#include <regex>
#include <iostream>
int main() {
    std::cout<<"start: "<<std::endl;
    std::regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::cout<<"end: "<<std::endl;
}
