#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    struct hostent *host;
    host = gethostbyname(argv[1]);

    if (!host) {
        printf("gethostbyname error");
        return 0;
    }

    int i;

    // 输出host
    printf("official name is :%s\n", host->h_name);
    for (i = 0; host->h_aliases[i]; i++) {
        printf("aliases %d: %s\n", i, host->h_aliases[i]);
    }
    printf("address type: %s\n", host->h_addrtype == AF_INET ? "ipv4" : "ipv6");
    for (i = 0; host->h_addr_list[i]; i++) {
        // 注意这里的h_addr_list保存了IP地址的list,但是这里使用了char *类型而不是in_addr,是因为为了兼容ipv4和ipv6
        // 没有使用void*的原因是定义套接字相关函数都是在void指针标准化之前定义的,而当时无法明确指出指针类型时采用的是char*
        printf("IP addr %d: %s\n", i, inet_ntoa(*(struct in_addr *) host->h_addr_list[i]));
    }
    return 0;
}