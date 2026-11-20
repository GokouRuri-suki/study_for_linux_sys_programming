// 下面是AI写的有名版本
/* ============================================================
 * 9.1 命名管道（FIFO）+ 命名信号量
 *
 * 就是 9.无名管道与信号量.c 的有名版本：
 *
 *            9                          9.1
 *   管道     pipe(pfd) / pipe(pfd2)     file/pfd / file/pfd2
 *   写端     pfd[1]                     pfd  = open(..., O_WRONLY)
 *   读端     pfd[0]                     pfd  = open(..., O_RDONLY)
 *   信号量   mmap + sem_init  st, st2   sem_open      st, st2
 *   关闭     -                          close
 *   清理     munmap                     unlink + sem_close
 *
 * 为什么要命名：没有共同祖先的进程拿不到同一份 fd，
 * 只能靠“名字”（文件路径）找到对方。
 *
 * open 会阻塞：O_WRONLY 等到有读者，O_RDONLY 等到有写者
 *   → 两边必须按【相同顺序】打开，否则互相等 → 死锁
 * ============================================================ */
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
//
//
#define FIFO_DIR "file"
#define FIFO_PFD "file/pfd"   /* 9 的 pfd */
#define FIFO_PFD2 "file/pfd2" /* 9 的 pfd2 */
#define SEM_ST "/p91_st"      /* 9 的 st */
#define SEM_ST2 "/p91_st2"    /* 9 的 st2 */
//
//
void doublePipe();
//
/* ── 这里的信号量为什么用 sem_open ───────────────────────
 * 9 里用的是 mmap 匿名共享内存 + sem_init，靠 fork 继承；
 * 9.1 假装没有血缘，拿不到同一份映射，只能靠名字：
 *   sem_open("/p91_st", ...)  →  /dev/shm/sem.p91_st
 *   sem_unlink("/p91_st")     →  名字删掉，映射还在
 *
 * 注意：sem_destroy 只能用于 sem_init 的对象，
 *       sem_open 拿到的指针要用 sem_close。
 * ─────────────────────────────────────────────────────── */
//
//
//
static void die(const char *what) {
  perror(what);
  exit(EXIT_FAILURE);
}
//
//
//
int main() {
  doublePipe();
  return 0;
};
//
//
//
void doublePipe() {
  /* 1. 建 FIFO（先清残留，否则 EEXIST） */
  if (mkdir(FIFO_DIR, 0755) != 0 && errno != EEXIST)
    die("mkdir");
  unlink(FIFO_PFD);
  unlink(FIFO_PFD2);
  if (mkfifo(FIFO_PFD, 0666) != 0)
    die("mkfifo pfd");
  if (mkfifo(FIFO_PFD2, 0666) != 0)
    die("mkfifo pfd2");

  /* 2. 命名信号量：先删残留 → 创建 → 立刻删名字 */
  sem_unlink(SEM_ST);
  sem_unlink(SEM_ST2);
  sem_t *st = sem_open(SEM_ST, O_CREAT, 0644, 0);
  sem_t *st2 = sem_open(SEM_ST2, O_CREAT, 0644, 0);
  if (st == SEM_FAILED || st2 == SEM_FAILED)
    die("sem_open");
  sem_unlink(SEM_ST);
  sem_unlink(SEM_ST2);

  int pfd, pfd2;
  char BUFF[1024], BUFF2[1024];
  pid_t pid2 = 0; // 父进程收尾要用，声明提到外面
  pid_t pid = fork();
  if (pid < 0)
    die("fork");
  else if (pid == 0) {              // 子
    pfd = open(FIFO_PFD, O_WRONLY); // = 9 的 pfd[1]
    if (pfd < 0)
      die("open pfd");
    pfd2 = open(FIFO_PFD2, O_RDONLY); // = 9 的 pfd2[0]
    if (pfd2 < 0)
      die("open pfd2");

    strcpy(BUFF, "Yes,我是1\0");
    write(pfd, BUFF, sizeof BUFF);
    sem_post(st);

    while (1) {
      sem_wait(st2);
      read(pfd2, BUFF2, sizeof BUFF2);
      printf("我是1，你说的是%s\n", BUFF2);
      break;
    }

    close(pfd);
    close(pfd2);
    exit(EXIT_SUCCESS);
  } else if (pid > 0) { // 父
    pid2 = fork();
    if (pid2 < 0)
      die("fork");
    else if (pid2 == 0) {             // 子2
      pfd = open(FIFO_PFD, O_RDONLY); // = 9 的 pfd[T]
      if (pfd < 0)
        die("open pfd");
      pfd2 = open(FIFO_PFD2, O_WRONLY); // = 9 的 pfd2[1]
      if (pfd2 < 0)
        die("open pfd2");

      strcpy(BUFF2, "YES,我是2\0");
      write(pfd2, BUFF2, sizeof BUFF2);
      sem_post(st2);

      while (1) {
        sem_wait(st);
        read(pfd, BUFF, sizeof BUFF);
        printf("我是2，你说的是%s\n", BUFF);
        break;
      }

      close(pfd);
      close(pfd2);
      exit(EXIT_SUCCESS);
    }
  }
  waitpid(pid, NULL, 0);
  waitpid(pid2, NULL, 0);
  sem_close(st);
  sem_close(st2);
  unlink(FIFO_PFD);
  unlink(FIFO_PFD2);
  return;
}
