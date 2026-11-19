/* ============================================================
 * 无名信号量 vs 有名信号量
 *
 * | 项目     | 无名信号量              | 有名信号量                      |
 * |----------|-------------------------|---------------------------------|
 * | 创建     | sem_init(&sem,0,val)    | sem_open("/n",O_CREAT,0644,val) |
 * | 销毁     | sem_destroy(&sem)       | sem_close()+sem_unlink()        |
 * | 标识     | 内存里的 sem_t 变量     | 内核里的一个名字                |
 * | 跨进程   | pshared=1 + 共享内存    | 天然可以                        |
 * | 生命周期 | 随进程消失              | 直到 sem_unlink                 |
 * | 类型     | sem_t                   | sem_t *                         |
 * ============================================================ */
// 主要是锁和信号量和PV操作什么的
#include <pthread.h>
#include <semaphore.h> //分有名和无名信号量(基于内存的信号量)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *semFunc(void *arg);
void baseOprAboutPV();

char BUFF[1024] = {0};
sem_t st;

int main() {
  baseOprAboutPV();
  return 0;
}

/**
 *
 *@brief两个线程同步读写缓冲区(生产者/消费者问题)
 *@note　首先是阻塞子线程的所以sem是0起始
 *
 *
 */
void baseOprAboutPV() {
  /*
  #include <semaphore.h>
  int sem_init(sem_t * sem, int pshared , unsigned int val);
  成功时返回0，失败时返回EOF
  sem 指向要初始化的信号量对象
  pshared   0表示线程间，1表示进程间
  val 表示信号量的初始值，如果为0表示没有资源，大于0表示有资源。

  #include <semaphore.h>
  //P操作
  int sem_wait(sem_t * sem);
  //V操作
  int sem_post(sem_t * sem);
   *
   *
   *
   *
   */

  void *ret;
  pthread_t pRead;
  if (sem_init(&st, 0, 0) < 0) {
    perror("信号量初始化异常\n");
    exit(-1);
  }
  if (pthread_create(&pRead, NULL, semFunc, NULL) != 0) {
    printf("子线程创建失败\n");
    exit(-1);
  }

  do {
    fgets(BUFF, 1024, stdin);
    sem_post(&st);
  } while (strncmp(BUFF, "quit", 4) != 0);
  printf("线程退出准备");
  pthread_join(pRead, &ret);
  printf("结果为%s", (char *)ret);

  return;
}
/**依
 *@brief子线程作为输出读取的v操作
 *
 */
void *semFunc(void *arg) {
  while (1) {
    if (sem_wait(&st) == 0)
      if (strncmp("quit", BUFF, 4) == 0) {

        pthread_exit((void *)("YES"));
      }
    printf("你输入了%zu个字符对吧!\n", strlen(BUFF) - 1);
    memset(BUFF, 0, 1024);
  }
}
