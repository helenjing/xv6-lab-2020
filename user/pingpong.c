#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void
mpingpong(){
    int p2c[2];
    int p2p[2];
    pipe(p2c);
    pipe(p2p);
    int pid=fork();
    if(pid==-1){
        exit(1);
    }else if(pid==0){   // child
        // close fid
        close(p2c[0]);
        close(p2p[1]);
        char ch='a';
        // send a byte
        write(p2c[1], &ch, sizeof(ch));

        char buffer[1];
        read(p2p[0], buffer, 1);
        pid=getpid();
        fprintf(1, "%d: received ping\n", pid);
        exit(0);
    }else{  // parent
        close(p2c[1]);
        close(p2p[0]);
        // send a byte
        char buffer[1];
        read(p2c[0], buffer, 1);
        write(p2p[1], buffer, sizeof(*buffer));
        pid=getpid();
        wait(0);
        fprintf(1, "%d: received pong\n", pid);
        exit(0);
    }
}
int
main (int argc, char* argv[])
{
    if(argc!=1) exit(1);
    mpingpong();
    exit(0);
}