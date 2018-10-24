//
// Created by 陈树沛 on 2018/10/24.
//

#ifndef AXIS_SIGNALHANDLER_H
#define AXIS_SIGNALHANDLER_H
//#define sockaddr struct sockaddr
//#define MAXLINE 4096
void sig_chld(int signo) {
    // signal handler
    pid_t pid;
    int stat;

    // pid = wait(&stat); 如果多个 SIGCHLD 信号同时到达，wait 只能获取第一个停止的进程
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) // WNOHANG 告知 waitpid 在有尚未终止的子进程在运行时不要阻塞
        std::cout << "Child " << pid << " terminated" << std::endl;

    return;
}
#endif //AXIS_SIGNALHANDLER_H
