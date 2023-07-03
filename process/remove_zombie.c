#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void handleZombie(int sig);

int main(int argc, char *argv[]) {
    pid_t pid;
    struct sigaction act;
    act.sa_handler = handleZombie;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if (pid == 0) {
        puts("this is chile process");
        sleep(10);
        return 1;
    } else {
        pid = fork();
        if (pid == 0) {
            puts("this is child process");
            sleep(10);
            exit(10);
        } else {
            printf("child pid is [%d]", pid);
            int i;
            for (i = 0; i < 5; i++) {
                puts("wait...");
                sleep(5);
            }
        }
    }

    return 0;
}

void handleZombie(int sig) {
    if (sig != SIGCHLD)
        return;
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);

    if (WIFEXITED(status)) {
        printf("child process pid is [%d]", pid);
        printf("exit ok.return [%d]\n", WEXITSTATUS(status));
    }
}