#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    if (argc <= 1) {
        fprintf(2, "sleep: missing operand\n");
        exit(1);
    }
    int i;
    for (i = 1; i < argc; ++i) {
        int t = atoi(argv[i]);
        sleep(t);
    }
    exit(0);
}

