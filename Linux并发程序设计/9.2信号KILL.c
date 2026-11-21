/* ============================================================
 * 附：常用信号速查（kill -l / man 7 signal）
 *
 * 编号  名称        默认动作    触发场景
 * ─────────────────────────────────────────────────────────
 *  1    SIGHUP      终止        终端关闭，发给会话内所有进程
 *  2    SIGINT      终止        Ctrl-C
 *  3    SIGQUIT     终止+core   Ctrl-\
 *  4    SIGILL      终止+core   非法指令
 *  6    SIGABRT     终止+core   abort()
 *  7    SIGBUS      终止+core   总线错误（对齐错、映射失效）
 *  8    SIGFPE      终止+core   算术异常（除零等）
 *  9    SIGKILL     终止        不可捕捉、不可忽略
 * 10    SIGUSR1     终止        用户自定义
 * 11    SIGSEGV     终止+core   非法访存（段错误）
 * 12    SIGUSR2     终止        用户自定义
 * 13    SIGPIPE     终止        写无读端管道 → 管道破裂
 * 14    SIGALRM     终止        alarm() / 定时器到期
 * 15    SIGTERM     终止        kill 默认信号，可捕捉
 * 17    SIGCHLD     忽略        子进程状态改变（退出/暂停）
 * 18    SIGCONT     继续        让暂停的进程恢复运行
 * 19    SIGSTOP     暂停        不可捕捉、不可忽略
 * 20    SIGTSTP     暂停        Ctrl-Z
 * 21    SIGTTIN     暂停        后台进程读终端
 * 22    SIGTTOU     暂停        后台进程写终端
 * 23    SIGURG      忽略        带外数据到达
 * 24    SIGXCPU     终止+core   超出 CPU 时间限制
 * 25    SIGXFSZ     终止+core   超出文件大小限制
 * 26    SIGVTALRM   终止        虚拟定时器
 * 27    SIGPROF     终止        profiling 定时器
 * 28    SIGWINCH    忽略        终端窗口大小变化
 * 29    SIGIO       终止        异步 I/O 事件
 * 30    SIGPWR      终止        电源故障
 * 31    SIGSYS      终止+core   非法系统调用
 * 34～64 SIGRTMIN~ 终止        实时信号，可排队、可带数据
 *
 * 两个固定规则：
 *   SIGKILL(9) / SIGSTOP(19) —— 既不能捕捉，也不能忽略
 *   32 / 33 被 glibc(NPTL) 占用，所以 SIGRTMIN 实际是 34
 ** ============================================================ */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void alarmOprAndPause();
void captureSigno();
void getSigno(int signo);
int main() {
  captureSigno();
  return 0;
}
//
//
//
//
//
void alarmOprAndPause() {
  printf("pid: %d\n", getpid()); // 打印自己的 PID
  alarm(3); // 3 秒后，内核给本进程发 SIGALRM 14默认是终止，杀掉
  pause();  // 挂起，直到收到【任意一个信号】才返回
  printf("I am wake up.\n"); // ← 执行不到
}
void captureSigno() {
  printf("pid:%d\n", getpid());
  printf("捕捉信号，转发函数getSigno\n");
  signal(SIGUSR2, getSigno);
  pause();
  printf("返回了\n");
  exit(EXIT_SUCCESS);
}
void getSigno(int signo) {
  if (signo == SIGUSR2) { // 这个是12
  }
}
