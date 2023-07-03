#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

void errorHandling(char *message);


int main(int argc, char *argv[]) {

    char message[] = "hello world!";
    // step1: 创建socket套接字
    // socket 参数
    // param1: PF_INET-IPV4 互联网协议族;PF_INET6-IPV6 互联网协议族;PF_LOCAL-本地通信 Unix 协议族;PF_PACKET-底层套接字的协议;PF_IPX=IPX Novel 协议族;
    // param2: SOCK_STREAM-面向连接的套接字(TCP);SOCK_DGRAM-面向消息的套接字(UDP);
    // param3: 决定最终使用的协议,可以应对同一协议族中存在的多个数据传输方式相同的协议，所以数据传输方式相同，但是协议不同，需要用第三个参数指定具体的协议信息；
    int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        errorHandling("socket() error");
    }

    struct sockaddr_in serve_addr;
    struct sockaddr_in client_addr;

    // 对于详细的地址信息使用sockaddr类型传递特别麻烦，进而有了sockaddr_in类型，其中基本与前面的类型保持一致，
    // 填充地址族、绑定网络接口、绑定监听的端口号,剩余全部填 0，所以强制转换后，不影响程序运行;
    memset(&serve_addr, 0, sizeof(serve_addr));
    serve_addr.sin_family = AF_INET;// AF_INET-IPV4用的地址族;AF_INET6-IPV6用的地址族;AF_LOCAL-本地通信中采用的 Unix 协议的地址族
    // 注意这里有大小端存储,统一转换成网络序-大端存储;
    serve_addr.sin_addr.s_addr = htonl(INADDR_ANY);// INADDR_ANY: 绑定所有可用的网络接口
    serve_addr.sin_port = htons(atoi(argv[1]));// 绑定监听的端口号

    // step2: socket绑定ip、端口
    if (bind(serverSocket, (struct sockaddr *) &serve_addr, sizeof(serve_addr)) == -1) {
        errorHandling("bind error");
    }

    // step3: 监听
    // 调用 listen 函数进入等待连接请求状态。连接请求状态队列的长度设置为5
    // 注意 此时的套接字才是服务端套接字
    if (listen(serverSocket, 5) == -1) {
        errorHandling("listen error");
    }

    // step4: 等待客户端连接
    socklen_t client_addr_size = sizeof(client_addr);

    int clientSocket = accept(serverSocket, (struct sockaddr *) &client_addr, &client_addr_size);
    if (clientSocket == -1) {
        errorHandling("accept error");
    }

    // step5: 向连接中写入数据
    write(clientSocket, message, sizeof(message));

    // step6: 关闭连接
    close(serverSocket);
    close(clientSocket);
    return 0;
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}