#include "kernel/types.h"
#include "user/user.h"


int
main(int ac, char **av)
{
    int ticks = uptime();
    printf("System has been running for %d ticks\n", ticks);
    exit(0);
}
