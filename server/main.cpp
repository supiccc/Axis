//
// Created by 陈树沛 on 2018/10/24.
//

#include "init.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cout << "服务已启动，端口号：9877，等待连接...." << "\n";
        start(9877);
    } else if(argc == 2) {
        char *p = argv[0];
        int port = atoi(p);
        if (port < 65525) {
            start(port);
        } else {
            std::cout << "端口号范围为0～65535" << "\n";
        }
    } else {
        std::cout << "启动错误！\n";
    }
}