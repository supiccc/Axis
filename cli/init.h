//
// Created by 陈树沛 on 2018/10/24.
//

#ifndef AXIS_INIT_H
#define AXIS_INIT_H
#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include "sendMessage.h"

//#define SERV_PORT 9877
//#define MAXLINE 4089
//#define sockaddr struct sockaddr
void start() {
    int sockfd;
    struct sockaddr_in servaddr;
    char addr[20];

    std::cout << "please enter IP address：";
    std::cin >> addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, addr, &servaddr.sin_addr);

    if(connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
        // 这里要使用 endl 刷新缓冲区！才能立即输出！
        std::cout << "welcome to supiccc's server!" << std::endl;
        str_cli(stdin, sockfd);
    }
}
#endif //AXIS_INIT_H
