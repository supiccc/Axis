//
// Created by 陈树沛 on 2018/10/24.
//

#ifndef AXIS_SENDMESSAGE_H
#define AXIS_SENDMESSAGE_H
#define SERV_PORT 9877
#define MAXLINE 4089
#define sockaddr struct sockaddr
void str_cli(FILE * fp, int sockfd) {
    fd_set rset;
    int stdineof = 0, maxfdp,n;
    char buff[MAXLINE];

    FD_ZERO(&rset);

    for (; ; ) {
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);
        maxfdp = std::max(fileno(fp), sockfd) + 1;
        select(maxfdp, &rset , NULL, NULL, NULL);

        if (FD_ISSET(fileno(fp), &rset)) {
            if ((n = read(fileno(fp), buff, MAXLINE)) == 0) {
                stdineof = 1;
                shutdown(fileno(fp), SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            } else {
                write(sockfd, buff, n);
            }
        }

        if (FD_ISSET(sockfd, &rset)) {
            if ((n = read(sockfd, buff, MAXLINE)) == 0) {
                if (stdineof == 0) {
                    std::cout << "服务器中断！";
                    return;
                } else {
                    std::cout << "str_cli: server terminated prematurely";
                    exit(0);
                }
            }
            write(fileno(stdout), buff, n);
        }
    }
}
#endif //AXIS_SENDMESSAGE_H
