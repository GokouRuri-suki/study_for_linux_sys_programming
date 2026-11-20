#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define PRODUCE_COUNT 20
#define CONSUME_COUNT 20

void *producer(void *arg);
void *consumer(void *arg);
void is_created(int x);
//

// 共享缓冲区
int buffer[BUFFER_SIZE];
int in = 0;  // 下一个生产的位置
int out = 0; // 下一个消费的位置
//
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_not_empty = PTHREAD_COND_INITIALIZER; // 有货 → 叫消费者
pthread_cond_t cond_not_full = PTHREAD_COND_INITIALIZER;  // 有空位 → 叫生产者
pthread_t cTid, pTid;
//
//
int main() {
  memset(buffer, 0, sizeof buffer);
  is_created(pthread_create(&cTid, NULL, consumer, NULL));
  is_created(pthread_create(&pTid, NULL, producer, NULL));

  pthread_join(cTid, NULL);
  pthread_join(pTid, NULL);

  // 销毁
  pthread_cond_destroy(&cond_not_empty);
  pthread_cond_destroy(&cond_not_full);
  pthread_mutex_destroy(&mutex);
  exit(EXIT_SUCCESS);
}
//
//
//
//

//
//
//
//
//
//
void *producer(void *arg) {
  for (int i = 1; i <= PRODUCE_COUNT; i++) {

    usleep(100000); /* 0.1 秒：生产者快，消费者慢 */
    pthread_mutex_lock(&mutex);
    while (((in + 1) % BUFFER_SIZE) == out) {
      pthread_cond_wait(&cond_not_full, &mutex);
    }
    buffer[in] = i;
    printf("生产者： %d 放入位置%d\n", i, in);
    in = (in + 1) % BUFFER_SIZE;
    pthread_cond_signal(&cond_not_empty);
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}
void *consumer(void *arg) {
  for (int i = 1; i <= CONSUME_COUNT; i++) {
    sleep(1);
    pthread_mutex_lock(&mutex);
    while (in == out) {
      pthread_cond_wait(&cond_not_empty, &mutex);
    }
    int item = buffer[out];
    printf("消费者 %d 从%d\n", item, out);
    out = (out + 1) % BUFFER_SIZE;
    pthread_cond_signal(&cond_not_full);
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}
void is_created(int x) {
  if (x != 0) {
    printf("创建失败\n");
    exit(EXIT_FAILURE);
  }
  return;
}
