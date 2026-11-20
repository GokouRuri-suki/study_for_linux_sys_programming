/* ============================================================
 * 进程间通信（IPC）之一：无名管道（pipe）
 *
 * Linux 主要 IPC 方式：
 *   管道 pipe / FIFO、消息队列、共享内存、信号量、信号、socket
 * 本篇只讲最基础的 pipe。
 *
 * ── 无名管道（pipe）数据流向 ─────────────────────────────
 *
 *        写进程                                读进程
 *   ┌──────────────┐                     ┌──────────────┐
 *   │  不使用 fd[0] │                     │  使用 fd[0]   │
 *   │  使用   fd[1] │                     │  不使用 fd[1] │
 *   └───────┬──────┘                     └───────▲──────┘
 *           │ write(fd[1], buf, n)               │ read(fd[0], buf, n)
 *           ▼                                    │
 *   ┌────────────────────────────────────────────┴─────┐
 *   │ 内核                                             │
 *   │        ┌────────────────────────┐                │
 *   │        │  管道（内核缓冲区）      │                │
 *   │        └────────────────────────┘                │
 *   └──────────────────────────────────────────────────┘
 *
 * 1. pipe(fd) 一次创建两个文件描述符，方向是写死的：
 *      fd[0] —— 读端，只能 read
 *      fd[1] —— 写端，只能 write
 *    所以写进程不碰 fd[0]，读进程不碰 fd[1]。
 *
 * 2. 管道本体在【内核】里，不属于任何进程的地址空间。
 *    write 把数据从用户态拷进内核缓冲区，read 再拷出来 —— 一共两次拷贝。
 *    这也是它比共享内存慢、但比文件简单的原因。
 *
 * 3. 半双工：一条管道只能单向传数据。
 *    要双向通信必须建两条管道（比如父→子一条、子→父一条）。
 *
 * 4. 无名管道只能用于【有亲缘关系】的进程（父子、兄弟），
 *    因为 fd 是靠 fork 继承的。不相干的进程要用命名管道 FIFO。
 *
 * 5. 两端都要把自己不用的那一头 close 掉：
 *      写进程：close(fd[0]);
 *      读进程：close(fd[1]);
 *    不关的后果：读端永远等不到 EOF。因为“所有写端都关闭”才会让
 *    read 返回 0，只要还有一个写端开着，read 就一直阻塞。
 *
 * 6. 阻塞语义：
 *      read  —— 管道为空且写端未全关 → 阻塞
 *               写端全部关闭          → 返回 0（EOF）
 *      write —— 缓冲区满 → 阻塞
 *               没有读端   → 触发 SIGPIPE（默认杀进程）
 *
 * 7. 容量与原子性（Linux）：
 *      默认缓冲区 64KB
 *      单次 write ≤ PIPE_BUF(4096) 时写入是原子的，不会被别的写者插队
 * ============================================================ */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
//
//
void doublePipe();
//
/* ── 这里的信号量为什么必须放共享内存 ───────────────────
 * 不能写成栈上的 sem_t：
 *   sem_t st;              // 栈上
 *   sem_init(&st, 1, 0);
 *   fork();                // 父子各拿到一份 st 的副本
 *   子进程 sem_post(&st);   // 只改了自己那份
 *   父进程 sem_wait(&st);   // 永远看到 0 → 死锁
 *
 * 本质：fork 复制地址空间（写时复制），不是"值传递"，
 *       但效果一样 —— 对方看不到你的修改。
 *
 * 两个容易混的点：
 *   1. sem_init 的 pshared=1 只是"声明这个信号量要给多进程用"，
 *      它本身不会把内存变成共享的；共享内存是 mmap 负责的。
 *   2. mutex 其实也能跨进程，但需要 PTHREAD_PROCESS_SHARED
 *      属性 + 共享内存，写起来比信号量麻烦，
 *      所以进程间同步一般首选信号量（mutex 默认只在线程间有效）。
 *
 * 本练习目的：用【管道 + 信号量】做先后控制（类似对讲机令牌），
 *             而不是用 sleep 赌时间，也不是用 mutex。
 * 这段共享内存写法是 AI 补的，还没系统学过，先照抄，
 * 只要记住一句：跨进程共享的东西，必须放在共享内存里。
 * ─────────────────────────────────────────────────────── */
//
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
  sem_t *st = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  sem_t *st2 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (st == MAP_FAILED || st2 == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  sem_init(st, 1, 0);
  sem_init(st2, 1, 0);
  int pfd[2], pfd2[2];
  char BUFF[1024], BUFF2[1024];
  if (pipe(pfd) != 0 || pipe(pfd2)) {
    printf("error");
    exit(EXIT_FAILURE);
  }
  pid_t pid2 = 0; // 父进程收尾要用，声明提到外面
  pid_t pid = fork();
  if (pid < 0) {

    printf("error");
    exit(EXIT_FAILURE);
  } else if (pid == 0) { // 子
    strcpy(BUFF, "Yes,我是1\0");
    write(pfd[1], BUFF, sizeof BUFF);
    sem_post(st);
    while (1) {
      sem_wait(st2);
      read(pfd2[0], BUFF2, sizeof BUFF2);
      printf("我是1，你说的是%s\n", BUFF2);
      break;
    }
    exit(EXIT_SUCCESS);
  } else if (pid > 0) { // 父
    pid2 = fork();
    if (pid2 < 0) {
      printf("pid2创建异常\n");
      exit(EXIT_FAILURE);
    } else if (pid2 == 0) { // 子2
      strcpy(BUFF2, "YES,我是2\0");
      write(pfd2[1], BUFF2, sizeof BUFF2);
      sem_post(st2);
      while (1) {
        sem_wait(st);
        read(pfd[0], BUFF, sizeof BUFF);
        printf("我是2，你说的是%s\n", BUFF);
        break;
      }
      exit(EXIT_SUCCESS);
    }
  }
  waitpid(pid, NULL, 0);
  waitpid(pid2, NULL, 0);
  sem_destroy(st);
  sem_destroy(st2);
  return;
}
