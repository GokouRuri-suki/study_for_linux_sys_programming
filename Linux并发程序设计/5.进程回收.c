#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

/**
 *pid_t wait(int * status); status
 指定保存子进程返回值和结束方式的地址
 *pid_t waitpid(pid_t pid, int *status, int option);
  pid==-1代表任意  option ==0 || WNOHANG
*/

/**
● 子进程通过exit/ _exit/return 返回某个值(0-255)  只有低八位
● 父进程调用wait(&status)   回收
  ○ WIFEXITED(status) 判断子进程是否正常结束（正常结束返回非0，异常结束返回0）
  ○ WEXITSTATUS(status)    获取子进程返回值
  ○ WIFSIGNALED(status)    判断子进程是否被信号结束
  ○ WTERMSIG(status)       获取结束子进程的信号类型
 */
int main() {
  pid_t pid = fork();
  int status;
  if (pid < 0) {
    perror("error about fork");
    return -1;
  }

  else if (pid == 0) {
    execlp("ls", "ls", "-a", "-l", NULL);

    // 根本不会执行，被exec族替换了进程
    sleep(1);
    printf("子进程退出 pid == %d\n", getpid());
  } else {
    sleep(3);
    printf("我是父进程\n");
    waitpid(pid, &status, 0);
    // wait(NULL);
    if (WIFEXITED(status)) // 如果正常结束就输出这句
    {
      printf("子进程的返回值为%d\n", WEXITSTATUS(status));
      printf("回收完毕\n");
    }
  }
  return 0;
}
// waitpid() 并不是“自动得到”子进程的 PID，而是因为父进程在调用
// fork()时，已经从返回值中获得了子进程的 PID。
