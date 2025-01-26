#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char buf[1024];
void runCommand(int cnt, char* args[], char* str){
    char* newargs[MAXARG];
    memset(newargs, 0, sizeof(char*));
    for(int i = 1; i < cnt; i++){
        newargs[i-1]=args[i];
    }
    newargs[cnt-1]=str;
    int pid=fork();
    if(pid<0){
        exit(1);
    }else if(pid>0){
        wait(0);
    }else{
        exec(args[1], newargs);
    }
}
void xargs(int cnt, char* args[], int fd){
    int n, m;
    char *p, *q;
    m=0;
    while((n = read(fd, buf+m, sizeof(buf)-m-1)) > 0){
        m += n;
        buf[m] = '\0';
        p = buf;
        while((q = strchr(p, '\n')) != 0){
            *q = 0;
            runCommand(cnt, args, p);
            p = q+1;
        }
        if(m > 0){
        m -= p - buf;
        memmove(buf, p, m);
        }
    }
}

int main(int argc, char* argv[]){
    if(argc < 3)    exit(1);    
    xargs(argc, argv, 0);   // read args from fd==0
    exit(0);
}