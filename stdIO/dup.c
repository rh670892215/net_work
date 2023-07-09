#include <stdio.h>
#include <unistd.h>

int main() {
    char str1[] = "hello\n";
    char str2[] = "world\n";

    int fd1 = dup(1);
    int fd2 = dup2(fd1, 5);

    printf("fd1 :%d,fd2 :%d\n", fd1, fd2);
    write(fd1, str1, sizeof(str1));
    write(fd2, str2, sizeof(str2));

    close(fd1);
    close(fd2);
    write(1, str1, sizeof(str1));
    close(1);
    write(1, str2, sizeof(str2));

    return 0;
}