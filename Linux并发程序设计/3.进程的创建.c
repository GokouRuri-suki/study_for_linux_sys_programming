#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

//
void father_child_process();
void exitAnd_exit();

int main() {
  father_child_process();

  return 0;
}

/**
 *@brief s创建子进程
 *
 ● 若父进程先结束
   ○ 子进程成为孤儿进程，被init进程收养
   ○ 子进程变成后台进程
 ● 若子进程先结束
   ○ 父进程如果没有及时回收，子进程变成僵尸进程。
 */
void father_child_process() {
  pid_t pid = fork();
  if (pid == -1) {
    perror("创建异常");
  } else if (pid == 0) {
    printf("我是子进程,pid = %d\n", getpid());
  } else {
    printf("我是父进程，pid = %d\n", getpid());
  }
  return;
}
/**
 *@ brief一个刷新缓冲一个不刷新,但是均立即终止,后半句都不输出
 *
 */
void exitAnd_exit() {
  printf("this process will exit");
  exit(0);
  //_exit();
  printf("never be displayed\n");
}
