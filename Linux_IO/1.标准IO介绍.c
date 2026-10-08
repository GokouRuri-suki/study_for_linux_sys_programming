#include <stdio.h>
#include <unistd.h>
/**
 *
 *note
 * stdout在linux中默认是行缓冲，那么
 * 一下代码1立即输出，2进入缓冲输出流然后和3一起输出;
 *结束main强制输出缓冲
 *
 */
int main() {
  fprintf(stdout, "1\n");
  sleep(3);
  fprintf(stdout, "2");
  sleep(3);
  fprintf(stdout, "3\n");
  fprintf(stdout, "4");

  return 0;
}
