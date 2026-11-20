#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *producerThread(void *arg); // 生产者：读输入写 buf
void *consumerThread(void *arg); // 消费者：读 buf 并打印
void CP();

char BUFF[1024] = {0};

// 生产者消费者模型的变量
pthread_t cPid, pPid;
sem_t sem_r, sem_w;

int main() {
  CP();
  return 0;
}

void *producerThread(void *arg) {
  do {
    sem_wait(&sem_w);
    fgets(BUFF, 1024, stdin);
    sem_post(&sem_r);
  } while (strncmp("Q", BUFF, 1) != 0);
  char *ret = "生产者结束了";
  pthread_exit((void *)ret);
} // 生产者：读输入写 buf
void *consumerThread(void *arg) {
  do {
    sem_wait(&sem_r);
    printf("您输入的是%s", BUFF);
    sem_post(&sem_w);
  } while (strncmp("Q", BUFF, 1) != 0);
  char *ret = "消费者下机了";
  pthread_exit((void *)ret);
} // 消费者：读 buf 并打印
void CP() {

  // 先写入然后在读取 再给写入权限往复
  if (sem_init(&sem_r, 0, 0) < 0) {
    perror("sem_init sem_r");
    exit(EXIT_FAILURE);
  }
  if (sem_init(&sem_w, 0, 1) < 0) {
    perror("sem_init sem_w");
    exit(EXIT_FAILURE);
  }
  if (pthread_create(&cPid, NULL, consumerThread, NULL) != 0) {
    printf("生产者异常\n");
    exit(-1);
  }
  if (pthread_create(&pPid, NULL, producerThread, NULL) != 0) {
    printf("消费者异常\n");
    exit(-1);
  }
  void *retC, *retP;
  pthread_join(cPid, &retC);
  pthread_join(pPid, &retP);
  printf("%s\n", (char *)retC);
  printf("%s\n", (char *)retP);
  sem_destroy(&sem_r);
  sem_destroy(&sem_w);
  return;
}
