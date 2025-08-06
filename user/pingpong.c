// user/pingpong.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv) {
    // 创建两个管道：pp2c 父到子，pc2p 子到父
    int pp2c[2], pc2p[2];
    pipe(pp2c); 
    pipe(pc2p); 
    
    if (fork() != 0) {                                // 父进程
        // 关闭父进程不需要的管道端
        close(pp2c[0]);   // 父进程不读pp2c
        close(pc2p[1]);   // 父进程不写pc2p

        // 父进程向子进程发送数据
        write(pp2c[1], ".", 1);
        close(pp2c[1]);   // 写完后关闭写端

        // 父进程从子进程接收数据
        char buf;
        read(pc2p[0], &buf, 1);
        printf("%d: received pong\n", getpid());
        close(pc2p[0]);   // 读完后关闭读端

        wait(0);  // 等待子进程结束
    } else {                                           // 子进程
        // 关闭子进程不需要的管道端
        close(pp2c[1]);   // 子进程不写pp2c
        close(pc2p[0]);   // 子进程不读pc2p

        // 子进程从父进程接收数据
        char buf;
        read(pp2c[0], &buf, 1);
        printf("%d: received ping\n", getpid());
        close(pp2c[0]);   // 读完后关闭读端

        // 子进程向父进程发送数据
        write(pc2p[1], &buf, 1);
        close(pc2p[1]);   // 写完后关闭写端
    }
    
    exit(0);
}
