#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char *argv[]) {
    struct iovec vec[2];
    char str1[] = "abcdefg";
    char str2[] = "1234567";

    vec[0].iov_base = str1;
    vec[0].iov_len = 3;
    vec[1].iov_base = str2;
    vec[1].iov_len = 3;

    int strLen = writev(1, vec, 2);
    puts("");
    printf("write bytes :%d\n", strLen);
    return 0;
}