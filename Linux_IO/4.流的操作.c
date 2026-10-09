#include <stdio.h>
/**
 *三个模式：
 *无缓冲，行缓冲，全缓冲
 *
 *本机linux下stdout是行缓冲
 *
 */

void setBuff();
int main() {
  setBuff();
  return 0;
}
/**
 *@brief设置缓冲模式
 *_IONBF,_IOLBF,_IOFBF
 @note全缓冲下只要阻塞就不输出，除非溢出（下例中用fgets()阻塞)
 */
void setBuff() {
  char buff[1024] = {0};
  int ret = setvbuf(stdout, buff, _IOFBF, 1024);
  printf("返回值：%d\n", ret);
  fputs("haha", stdout);
  fprintf(stdout, "test......\n");
  fgetc(stdin);
}
