#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//
//

void baseMain();
void *incThread(void *arg);
//
int shared_num = 0;
pthread_mutex_t mutex;
int main() {
  baseMain();
  return 0;
}
//
//
//
void baseMain() {
  if (pthread_mutex_init(&mutex, NULL) != 0) {
    printf("互斥锁初始化异常\n");
    exit(EXIT_FAILURE);
  }
  pthread_t tid1, tid2;
  if (pthread_create(&tid1, NULL, incThread, NULL) != 0) {
    printf("创建线程异常\n");
    exit(EXIT_FAILURE);
  }
  if (pthread_create(&tid2, NULL, incThread, NULL) != 0) {
    printf("创建线程异常\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  printf("结果为%d", shared_num);

  pthread_mutex_destroy(&mutex);
  exit(EXIT_SUCCESS);
}
void *incThread(void *arg) {
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < 10000; i++)
    shared_num += 1;
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

//
//
//
//
//
//
