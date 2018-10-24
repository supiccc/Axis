//
// Created by 陈树沛 on 2018/10/24.
//

#ifndef AXIS_MESSAGEHANDLER_H
#define AXIS_MESSAGEHANDLER_H
#include <map>
#include <string>
#include <algorithm>
#include <unordered_map>

#define sockaddr struct sockaddr
#define MAXLINE 4096

std::unordered_map<std::string, std::string> map;

bool isValid(char * buff, std::string& key, std::string & val, std::string& command, ssize_t size) {
    if(buff[3] != ' ') return false;
    // 获取命令
    for (int i = 0; i < 3; ++i) { command += buff[i]; }
    // 将命令转换为小写
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);
    if (command == "get") {
        int i = 4;
        while (buff[i] != ' ' && buff[i] != '\n') {
            key += buff[i++]; // 获取key
        }
        while (buff[i] == ' ') i++; // 清除空格
        return buff[i] == '\n'; // 判断是否结束
    } else if (command == "set") {
        int i = 4;
        while (buff[i] != ' ' && buff[i] != '\n') {
            key += buff[i++];
        }
        if (buff[i] != ' ') {
            return false;
        } else {
            i++;
        }
        if (buff[i] == '\n' || buff[i] == ' ') {
            return false;
        }
        while (buff[i] != ' ' && buff[i] != '\n') {
            val += buff[i++];
        }
        while (buff[i] == ' ') i++;
        return buff[i] == '\n';
    } else {
        return false;
    }
}
void str_echo(int sockfd) {
    ssize_t n;
    char buf[MAXLINE];
    long arg1, arg2;
    again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {
        std::string key, val, command;
        if (!isValid(buf, key, val, command, n)) {
            write(sockfd, "ERROR\n", 6); // 命令格式错误，返回客户端错误消息
        } else {
            if (command == "set") {
                if (map.count(key)) {
                    map[key] = val; // 修改
                    std::cout << key << " " << val << "\n";
                } else {
                    map.insert(std::make_pair(key, val)); // 新增
                    std::cout << key << " " << val << "\n";
                }
                write(sockfd, "OK\n", 3);
            } else {
//                std::cout << key << " " << val << "\n";
                if (map.count(key)) {
                    std::string res = map[key];
                    res += '\n';
                    auto len = (size_t)res.size();
//                    std::cout << "res";
                    write(sockfd, res.c_str(), len);
                } else {
                    write(sockfd, "No this entry\n", 14);
                }
            }
        }
    }

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        std::cout << "str_echo: read error";
}
#endif //AXIS_MESSAGEHANDLER_H
