/* ============================================================
 * 条件变量（pthread_cond_t）
 *
 * | 操作     | 函数                              | 说明             |
 * |----------|-----------------------------------|------------------|
 * | 初始化   | pthread_cond_init(&cond, NULL)    | 或宏初始化       |
 * | 等待     | pthread_cond_wait(&cond, &mutex)  | 必须持有 mutex   |
 * | 唤醒一个 | pthread_cond_signal(&cond)        | 叫醒一个等待者   |
 * | 唤醒全部 | pthread_cond_broadcast(&cond)     | 叫醒所有等待者   |
 * | 销毁     | pthread_cond_destroy(&cond)       | 和 init 配对     |
 * ============================================================ */
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//
//

void baseMain();
void *printOdd(void *arg);
void *printEven(void *arg);
//
#define MAX 20
int current = 1;
pthread_mutex_t mutex;
pthread_cond_t cond;
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
  if (pthread_cond_init(&cond, NULL) != 0) {
    printf("条件变量初始化异常\n");
    exit(EXIT_FAILURE);
  }
  pthread_t tid1, tid2;
  if (pthread_create(&tid1, NULL, printOdd, NULL) != 0) {
    printf("创建线程异常\n");
    exit(EXIT_FAILURE);
  }
  if (pthread_create(&tid2, NULL, printEven, NULL) != 0) {
    printf("创建线程异常\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  printf("\n");

  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);
  exit(EXIT_SUCCESS);
}
void *printOdd(void *arg) {
  while (1) {
    pthread_mutex_lock(&mutex);
    // 还没结束，但当前是偶数 -> 不是我的回合，等
    while (current <= MAX && current % 2 == 0)
      pthread_cond_wait(&cond, &mutex);
    // 醒来后判断是不是已经打完了
    if (current > MAX) {
      pthread_mutex_unlock(&mutex);
      break;
    }
    printf("%d ", current);
    current++;
    // 叫醒偶数线程
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}
void *printEven(void *arg) {
  while (1) {
    pthread_mutex_lock(&mutex);
    // 还没结束，但当前是奇数 -> 不是我的回合，等
    while (current <= MAX && current % 2 != 0)
      pthread_cond_wait(&cond, &mutex);
    if (current > MAX) {
      pthread_mutex_unlock(&mutex);
      break;
    }
    printf("%d ", current);
    current++;
    // 叫醒奇数线程
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}
//
//
//
//
//
//
