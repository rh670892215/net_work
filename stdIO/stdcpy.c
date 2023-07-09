#include <stdio.h>
#include <fcntl.h>

#define BUF_SIZE 3

int main() {
    FILE *f1;
    FILE *f2;

    f1 = fopen("news.txt", "r");
    f2 = fopen("cpy2.txt", "w");

    char buf[BUF_SIZE];
    while (fgets(buf, sizeof(buf), f1)) {
        fputs(buf, f2);
    }

    return 0;
}