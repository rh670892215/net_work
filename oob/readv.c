#include <stdio.h>
#include <sys/uio.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    struct iovec vec[2];

    char str1[BUF_SIZE] = {0,};
    char str2[BUF_SIZE] = {0,};

    vec[0].iov_base = str1;
    vec[0].iov_len = 5;
    vec[1].iov_base = str2;
    vec[1].iov_len = BUF_SIZE;

    int strLen = readv(0, vec, 2);
    printf("read bytes %d\n", strLen);
    printf("read str1 %s\n", vec[0]);
    printf("read str2 %s\n", vec[1]);
    return 0;
}