#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// func
void *worker(void *arg);
void is_suc(int flag);

#define THREAD_NUM 3

int DATA[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
typedef struct {
  int begin;
  int end;
} Task;

int main(void) {
  pthread_t sid[THREAD_NUM];
  Task task[THREAD_NUM] = {{0, 3}, {4, 7}, {8, 11}};
  void *ret[THREAD_NUM];
  int total = 0;

  for (int i = 0; i < THREAD_NUM; i++)
    is_suc(pthread_create(&sid[i], NULL, worker, &task[i]));

  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_join(sid[i], &ret[i]);
    int sum = (int)(intptr_t)ret[i];
    printf("第%d组%d\n", i + 1, sum);
    total += sum;
  }

  printf("总计%d\n", total);
  return 0;
}

void *worker(void *arg) {
  Task *t = (Task *)arg;
  int sum = 0;

  for (int i = t->begin; i <= t->end; i++)
    sum += DATA[i];

  pthread_exit((void *)(intptr_t)sum);
}

void is_suc(int flag) {
  if (flag != 0) {
    printf("创建有误！\n");
    exit(-1);
  }
  return;
}
