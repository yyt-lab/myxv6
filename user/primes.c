#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child_check(int p[2])
{
    int pp[2];

    pipe(pp);// with right neighbour
    int basenum ;
    int n;
    close(p[1]);  // * 被调用的子进程 先关闭一个自己不要的连接
    // ! 读取时必须关闭写入端口
    n = read(p[0],&basenum,sizeof(int));
    // close(p[1]);
    if (n<=0){
        close(p[0]);
        close(pp[0]);
        close(pp[1]);
        exit(0);
        // return ;
    }
    if (fork() == 0){
        close(p[0]);
        // close(pp[1]);
        // printf("12");
        child_check(pp);
    } else {
        // close(pp[0]);
        close(pp[0]);
        printf("prime %d\n",basenum);
        int newnum;
        while((read(p[0],&newnum,sizeof(int))) == 4){
            if (newnum % basenum != 0){
                write(pp[1],&newnum,sizeof(int));  // * 写入必须关闭读端口
            }
        }
        close(p[0]);
        close(pp[1]);
        // printf("newnum %d\n",newnum);
        wait(0);
    }
    exit(0);
}
int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if (fork()==0){
        // close(p[0]);
        // close(p[1]);
        child_check(p);
    }else {
        close(p[0]);
        for (int i=2;i<=35;i++){
            write(p[1],&i,sizeof(int));
            // printf("main:%d\n",*(&i));
        }
        close(p[1]);
    }
    wait(0);
    exit(0);
}