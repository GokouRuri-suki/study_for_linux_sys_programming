/*
 *主要是在进程中怎么执行命令的教学
 *
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
/*
● 进程调用exec函数族执行某个程序
● 进程当前内容被指定的程序替换
● 实现让父子进程执行不同的程序
  ○ 父进程创建子进程
  ○ 子进程调用exec函数族
  ○ 父进程不受影响
 */
void execlAndExelp();

int main() {
  // system("ls -la");
  execlAndExelp();
  return 0;
}
/**
 *@brief 需要完整路径 execl
 *       默认是path/  execlp
 *
 */
void execlAndExelp() {
  if (execl("/bin/ls", "ls", "-a", "-l", NULL) == -1) {
    perror("无权限或者不存在");
    return;
  }
  // 上面的成功执行了之后，进程会被替换，下面的会被忽略
  printf("\n这个是第二个指令：:");
  if (execlp("pwd", "pwd", NULL) == -1) {
    perror("出错了 无权限吗?");
    return;
  }

  char *args[] = {"ls", "-a", "-l", NULL};
  if (execv("/bin/ls", args) == -1) {
    perror("错误");
    return;
  }
  if (execvp("ls", args) == -1) {
    perror("错误");
    return;
  }

  // 以下是关于system函数与exec族不同的地方
  // system是内部fork的阻塞函数，而是内部fork，经过shell
  // 原进程不会被替换，命令执行完后继续运行。
  system("ls -la");
}
