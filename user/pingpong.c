#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
//   char arr_a[2]={'A'};
//   char arr_b[2]={'B'};
//   char buf[5];

      int p[2];
	  int p2[2];
      pipe(p); // ! 用于 子->父
      pipe(p2);// ! 用于 父->子
      if (fork()==0){ // ! 子进程
		// close(p[1]);
        // read(p[0],buf,1);
        fprintf(2,"%d: received ping\n",getpid());
		// close(p[0]);
		// close(p2[0]);
        // write(p2[1],arr_b,1);
		// exit(0);
      }else { // ! 父进程
		//   close(p[0]);
        //   write(p[1],arr_a,1);
		  wait(0);  
		//   close(p2[1]);
            // read(p2[0],buf,1);

          fprintf(2,"%d: received pong\n",getpid());   
  }  
		  exit(0);     
}


// int 
// main(int argc, char** argv ){
//     int pid;
//     int parent_fd[2];
//     int child_fd[2];
//     char buf[20];
//     //为父子进程建立管道
//     pipe(child_fd); 
//     pipe(parent_fd);

//     // Child Progress
//     if((pid = fork()) == 0){
//         close(parent_fd[1]);
//         read(parent_fd[0],buf, 4);
//         printf("%d: received %s\n",getpid(), buf);
//         close(child_fd[0]);
//         write(child_fd[1], "pong", sizeof(buf));
//         exit(0);
//     }
//     // Parent Progress
//     else{
//         close(parent_fd[0]);
//         write(parent_fd[1], "ping",4);
//         close(child_fd[1]);
//         read(child_fd[0], buf, sizeof(buf));
//         fprintf(2,"%d: received %s\n", getpid(), buf);
//         exit(0);
//     }
    
// }
