/*
● ps 查看系统进程快照
● top 查看进程动态信息
● /proc 查看进程详细信息
 */
void commandAboutTerminal();

int main() {
  commandAboutTerminal();
  return 0;
}
/**
 * TN暂停低优先
 * S+睡眠前台运行
 *
 *
 *
 */
void commandAboutTerminal() {
  /**
   * ps -ef | more
   * ps -ef | grep test
   * kill pid
   * kill %n
   * top
   * /proc
   *
   *
   *
   *
   *
   *  nice -n 2 ./test  优先级加2启动
   *  renice -n 4 23275 改变进程的优先级
   *  jobs查看后台进程
   *  fg放到进程前台执行
   *
   */
  while (1) {
  }
  /*
   *.test &
   * jobs
   *fg %./test
   *ctrl+z
   *bg%./test
   kill %./test
   */
}
