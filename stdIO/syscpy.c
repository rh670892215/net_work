#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 3

int main() {
    int fd1, fd2;
    char buf[BUF_SIZE];

    fd1 = open("news.txt", O_RDONLY);
    fd2 = open("cpy1.txt", O_WRONLY | O_CREAT | O_TRUNC);

    int len;
    while ((len = read(fd1, buf, sizeof(buf))) > 0) {
        write(fd2, buf, len);
    }
    close(fd1);
    close(fd2);
    return 0;
}