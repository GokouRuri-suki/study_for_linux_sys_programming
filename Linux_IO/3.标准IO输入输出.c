#include <stdio.h>
#include <unistd.h>

/*
 *发现到一个IO流中如果输入了abc\n
 *那么以后的stdin都是顺序读取，代码如下
 *无需输入fgetc的值会直接读取缓冲区域的b
 *失败返回-1(EOF)
 *
 *以下为了简单没判断函数的返回值是否为EOF
 */
void cpy_file();
void read_and_output();
int main() {
  // 标准IO
  printf("getchar() 的测试：");
  char getCHAR = getchar();
  printf("%c     ---  over\n", getCHAR);

  // getc()和fgetc()几乎一致，但是getc()不可传入表达式类
  printf("fgetc()：  \n ");
  getCHAR = fgetc(stdin);
  printf("%c     ---  over\n", getCHAR);

  // 文件IO
  FILE *fp = fopen("file/test.txt", "r");
  if (fp == NULL) {
    perror("错误信息：");
    return -1;
  }
  int flag = 0;
  while ((flag = fgetc(fp)) != EOF) {
    putchar(flag);
    // fprintf(stdout, "%c", (char)flag);
  }
  fclose(fp);
  // cpy_file();

  printf("fgets");
  FILE *fp_read = fopen("file/test.txt", "r");
  char buff[4096] = {0};
  char *ch_ptr = NULL;
  while ((ch_ptr = fgets(buff, sizeof(buff), fp_read)) != NULL) {
    fprintf(stdout, "%s", buff);
  }
  fclose(fp_read);
  printf("fgets over");

  // 字节级复制读写
  read_and_output();
  return 0;
}

void cpy_file() {
  FILE *fp_cpy = fopen("file/test_cpy", "w");
  FILE *fp = fopen("file/test.txt", "r");
  int flag = 0;
  while ((flag = fgetc(fp)) != EOF) {
    // fprintf(fp_cpy, "%c", (char)flag);
    putc((char)flag, fp_cpy);
  }
  fclose(fp);
  fclose(fp_cpy);
}
/**
 *@note注意下面的fwirte的参数2,3是倒置了的
 *这不是错误，是一种保证读取写入同步并且简便的写法
 *
 */
void read_and_output() {
  // 字节级复制读写
  FILE *fp_read = fopen("file/png.png", "rb");
  FILE *fp_write = fopen("file/new.png", "wb");
  char buf[4096] = {0};
  int byte_read = 0;
  while ((byte_read = fread(buf, 1, 5, fp_read)) > 0) {
    fwrite(buf, 1, byte_read, fp_write);
  }
  fclose(fp_write);
  fclose(fp_read);
}
