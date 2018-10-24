//
// Created by 陈树沛 on 2018/10/24.
//

#ifndef AXIS_INIT_H
#define AXIS_INIT_H
#include <sys/socket.h>
#include <iostream>
#include <cstdio>
#include <netinet/in.h>
#include <unistd.h>
#include <csignal>
#include <sys/wait.h>
#include <sys/select.h>
#include "signalHandler.h"
#include "messageHandler.h"
#define sockaddr struct sockaddr
#define MAXLINE 4096


void start(int port) {
    int listenfd, connfd, udpfd;
    struct sockaddr_in cliaddr, servaddr;
    socklen_t clilen;
    fd_set rset;
    int maxfd, nready;
    int nread;
    char buf[MAXLINE];
    pid_t childpid;
    const int on = 1;

    // create listening TCP sockets
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bind(listenfd, (sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, 1024);

    // crete listening UDP sockets
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9877);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(udpfd, (sockaddr *)&servaddr, sizeof(servaddr));

    std::signal(SIGCHLD, sig_chld); // 信号捕获处理函数 signal handler，处理僵死进程

    maxfd = std::max(udpfd, listenfd);

    FD_ZERO(&rset);


    while (1) {
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);

        if ((nready = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR)
                continue;
            else {
                printf("select error");
                exit(0);
            }
        }

        // 当服务器收到一个 SYN 信号时，listenfd 变为可读状态
        if (FD_ISSET(listenfd, &rset)) {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (sockaddr *)&cliaddr, &clilen);
            if ((childpid = fork()) == 0) {
                close(listenfd);
                str_echo(connfd);
                exit(0);
            }
            close(connfd);
        }

        if (FD_ISSET(udpfd, &rset)) {
            clilen = sizeof(cliaddr);
            nread = recvfrom(udpfd, buf, MAXLINE, 0, (sockaddr *)&cliaddr, &clilen);
            sendto(udpfd, buf, nread, 0, (sockaddr *)&cliaddr, clilen);
        }
    }
}
#endif //AXIS_INIT_H
