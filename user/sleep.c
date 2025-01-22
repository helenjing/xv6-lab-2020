#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void
mysleep(char *seconds){
    int sec = atoi(seconds);
    sleep(sec);
}

int
main(int argc, char *argv[])
{
    if(argc <= 1 || argc > 2){
        exit(1);
    }
    mysleep(argv[1]);
    exit(0);
}