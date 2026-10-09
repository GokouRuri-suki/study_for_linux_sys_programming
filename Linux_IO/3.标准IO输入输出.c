#include <stdio.h>
#include <unistd.h>

/*
 *发现到一个IO流中如果输入了abc\n
 *那么以后的stdin都是顺序读取，代码如下
 *无需输入fgetc的值会直接读取缓冲区域的b
 *
 *
 *
 *
 *
 *
 */
int main() {
  // 标准IO
  printf("getchar() 的测试：");
  char getCHAR = getchar();
  printf("%c     ---  over\n", getCHAR);

  printf("fgetc()：  \n ");
  getCHAR = fgetc(stdin);
  printf("%c     ---  over\n", getCHAR);

  // 文件IO
  FILE *fp = fopen("file/test.txt", "r");
  if (fp == NULL) {
    perror("错误信息：");
    return -1;
  }
  char tmp;
  while ((tmp = fgetc(fp)) != EOF) {
    fprintf(stdout, "%c", tmp);
  }
  fclose(fp);
  return 0;
}
