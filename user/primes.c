#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
pipeline(int p[2]){
    // fprintf(1, "I'm pid: %d", getpid());
    close(p[1]);
    int n=4;
    int curPrime;
    int num;
    n = read(p[0], &curPrime, sizeof(int)); // blocked?
    // fprintf(1, "I'm not blocked by read 1st: %d\n", n);
    if(!n){
        close(p[0]);
        exit(0);
    }  
    fprintf(1, "prime %d\n", curPrime);
    n = read(p[0], &num, sizeof(int));
    // fprintf(1, "I'm not blocked by read 2nd: %d\n", n);
    if(!n){
        close(p[0]);
        exit(0);
    }  
    int np[2];
    pipe(np);
    int pid=fork();
    if(pid<0){
        exit(1);
    }else if(pid==0){   // child
        pid = getpid();
        // fprintf(1, "child pid: %d start\n", pid);
        pipeline(np);
        close(p[0]);
        close(np[0]);
        close(np[1]);
        exit(0);
    }else{  // parent
        close(np[0]);
        while(n){
            if(num%curPrime){
                write(np[1], &num, sizeof(int));
            }
            n = read(p[0], &num, sizeof(int));
        }
        close(np[1]);
        wait(0);
        close(p[0]);
        // fprintf(1, "child pid: %d finished\n", pid);
        exit(0);
    }
}

void
myPrime(int st, int ed){
    int p[2];   // read, write
    pipe(p);
    int pid=fork();
    if(pid<0){
        exit(1);
    }else if(pid==0){   // child
        pipeline(p);
        exit(0);
    }else{  // parent
        close(p[0]);
        for(int i =st; i < ed+1; i++){
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
}

int
main(int argc, char* argv[]){
    if(argc>=2) exit(1);
    myPrime(2, 35);
    // myPrime(2, 10);
    exit(0);
}