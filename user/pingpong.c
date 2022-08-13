#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char* argv[])
{
    int pipe1[2];
    int pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    int pid = fork();
    if (pid == 0) {  // child
        close(pipe1[1]);
        close(pipe2[0]);
        int childPid = getpid();
        char buf[8];
        int n = read(pipe1[0], buf, 8);
        if (n > 0) {
            fprintf(1, "%d: received ping\n", childPid);
            write(pipe2[1], "0", 1);
        }
        close(pipe1[0]);
        close(pipe2[1]);
    } else {
        close(pipe1[0]);
        close(pipe2[1]);
        write(pipe1[1], "0", 1);
        int parentPid = getpid();
        char buf[8];
        int n = read(pipe2[0], buf, 8);
        if (n > 0) {
            fprintf(1, "%d: received pong\n", parentPid);
        }
        close(pipe1[1]);
        close(pipe2[0]);
    }
    exit(0);
}


