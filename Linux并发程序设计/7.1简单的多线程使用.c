#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
// func
void *func_1(void *arg);
void *func_2(void *arg);
void *func_3(void *arg);
void is_suc(int flag);

int DATA[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
typedef struct {
  int begin;
  int end;
} Task;

//
//
//
int main() {
  pthread_t sid_1, sid_2, sid_3;
  Task task1, task2, task3;
  void *ret1, *ret2, *ret3;
  task1.begin = 0, task1.end = 3;
  task2.begin = 4, task2.end = 7;
  task3.begin = 8, task3.end = 11;

  is_suc(pthread_create(&sid_1, NULL, func_1, &task1));

  is_suc(pthread_create(&sid_2, NULL, func_2, &task2));
  is_suc(pthread_create(&sid_3, NULL, func_3, &task3));
  pthread_join(sid_1, &ret1);
  pthread_join(sid_2, &ret2);
  pthread_join(sid_3, &ret3);

  printf("第一组%d\n", (int)(intptr_t)ret1);
  printf("第二组%d\n", (int)(intptr_t)ret2);
  printf("第三组%d\n", (int)(intptr_t)ret3);
  printf("总计%d\n", (int)(intptr_t)ret1 + (int)(intptr_t)ret2 + (int)(intptr_t)ret3);
  return 0;
};
//
//
//
void *func_1(void *arg) {
  Task *t = (Task *)arg;
  int sum = 0;

  for (int i = t->begin; i <= t->end; i++)
    sum += DATA[i];

  pthread_exit((void *)(intptr_t)sum);
}
void *func_2(void *arg) {
  Task *t = (Task *)arg;
  int sum = 0;

  for (int i = t->begin; i <= t->end; i++)
    sum += DATA[i];

  pthread_exit((void *)(intptr_t)sum);
}
void *func_3(void *arg) {
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
