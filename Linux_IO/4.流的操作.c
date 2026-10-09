#include <stdio.h>
/**
 *三个模式：
 *无缓冲，行缓冲，全缓冲
 *
 *本机linux下stdout是行缓冲
 *
 */

void setBuff();
void streamPositioning();
void streamError();
int main() {
  // setBuff(); // 设置换缓冲模式测试缓冲流
  //  streamPositioning();
  streamError();
  return 0;
}

/**
 * @brief设置缓冲模式
 * _IONBF,_IOLBF,_IOFBF
 * @note全缓冲下只要阻塞就不输出，除非溢出（下例中用fgets()阻塞)
 */
void setBuff() {
  char buff[1024] = {0};
  int ret = setvbuf(stdout, buff, _IOFBF, 1024);
  printf("返回值：%d\n", ret);
  fputs("haha", stdout);
  fprintf(stdout, "test......\n");
  fgetc(stdin);
}

/**
 * @brief流定位测试
 * long  ftell(FILE *stream);
 * long  fseek(FILE *stream, long offset, int whence);
 * void  rewind(FILE* stream);
 * pftell返回指定流当前的offset值;失败返回EOF
 * pfseek设置指定流的offset值;成功返回0;失败返回EOF
 * prewind将流的offset重置到开头;无返回值
 *
 *
 */
void streamPositioning() {
  FILE *fp = fopen("file/test.txt", "r");
  if (fp == NULL) {
    perror("异常错误:");
    return;
  }

  printf("打开成功当前的流定位的ftell是  %ld\n", ftell(fp));
  char *flag = NULL;
  char buff[4096] = {0};
  while ((flag = fgets(buff, 4060, fp)) != NULL) {
    fputs(buff, stdout);
    printf("\t\t\t当前位置是ftell  %ld\n", ftell(fp));
  }
  printf("\n\n");

  printf("再来一次(rewind重置到头部\n");
  printf("打开成功当前的流定位的ftell是  %ld\n\n", ftell(fp));

  rewind(fp);
  // fseek(fp,0,SEEK_SET);等价
  while ((flag = fgets(buff, 4060, fp)) != NULL) {
    printf("当前位置是ftell  %ld\n", ftell(fp));
  }
  fseek(fp, 56, SEEK_SET);
  if ((flag = fgets(buff, 4096, fp)) != NULL) {
    fputs(buff, stdout);
  }
  fclose(fp);
}
/**
 * @brief文件IO的错误判断
 * int ferror(FILE* stream);是否炸了？
 * int feof(FILE* stream);是否正常结尾？
 */
void streamError() {
  // ferror的使用
  FILE *fp = fopen("file/test.txt", "r");
  if (fp == NULL) {
    perror("error:  ");
    return;
  }
  if (ferror(fp)) {
    perror("文件读取异常：");

  } else {
    printf("读取正常\n");
  }
  fclose(fp);
  // feof()的使用
  FILE *fpp = fopen("file/test.txt", "r");
  if (fpp == NULL || ferror(fp)) {
    perror(" error");
    return;
  }
  char *flag = NULL;
  char buff[1024] = {0};
  while ((flag = fgets(buff, sizeof(buff), fp)) != NULL) {
    printf("是否已经读完%s\n", feof(fpp) ? "YES" : "NO");
  }
  printf("是否已经读完%s\n", feof(fpp) ? "YES" : "NO");

  fclose(fpp);
}
