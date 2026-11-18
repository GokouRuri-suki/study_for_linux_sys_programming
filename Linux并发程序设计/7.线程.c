/**
*
*
●
进程在切换时系统开销大CPU切换线程时需要重新将指令加载到Cache缓存中，并且页表项TLB也要做更新，每次切换开销非常大。频繁切换cache、tlb，内存对于cpu来说是慢速设别，需要将指令和数据复制到cache中

● 操作系统引入了轻量级进程LWP（线程）
●
同一个进程中的线程，共享相同的内存空间，即切换线程不会切换cache和tlb（或更新小部分）
● Linux不区分进程、线程（都是任务，即task struct）


线程共享资源
● 一个进程中的多个线程共享以下资源
  ○ 可执行的指令
  ○ 静态数据
  ○ 进程中打开的文件描述符
  ○ 当前工作目录
  ○ 用户ID
  ○ 用户组ID
线程私有资源
● 每个线程私有资源包括
  ○ 线程ID（TID)
  ○ PC(程序计数器)和相关寄存器
  ○ 堆栈
  ○ 错误号（errno)
  ○ 优先级
  ○ 执行状态和属性



进程 = 一个线程组（1 个或多个 task）+ 该组共享的资源
线程 = 一个 task + 它私有的执行上下文
调度单位是 task（LWP），资源归属单位是线程组；组长只是组里第一个 task


*/                                                                             \
#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

char str[1024] = "hello world\0";
void *fuc(void *arg);

int main() {
  pthread_t sid;
  void *ret;
  int is_suc = pthread_create(&sid, NULL, fuc, str);
  if (is_suc != 0) {
    printf("创建失败\n");
    exit(-1);
  }
  pthread_join(sid, &ret);
  printf("线程的执行结果为%s", (char *)ret);
  return 0;
}

void *fuc(void *arg) {
  char *retV = (char *)str;
  memset(retV, 0, strlen(retV));
  printf("这是线程 %u\n", getpid());
  strcpy(retV, "yes u a right!\0");
  printf("完成！准备结束释放\n");
  pthread_exit(retV);
}
