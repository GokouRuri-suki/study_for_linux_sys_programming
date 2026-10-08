#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
/**

 模式         	含义          	文件是否存在的要求
“r”或“rb”     	只读打开      	文件必须存在
“r+”或“r+b”   	可读可写打开  	文件必须存在，从文件起始位置写入
“w”或“wb”     	仅写打开      	不存在则创建；存在会清空原有内容
“w+”或“w+b”   	可读可写打开  	不存在则创建；存在会清空原有内容
“a”或“ab”     	仅写打开
不存在则创建；如已存在且有内容，在文件末尾追加新内容 “a+”或“a+b”
可读可写打开	不存在则创建；如已存在且有内容，写入会在文件末尾追加新内容

*/
int main() {
  FILE *fp = fopen("./file/test.txt", "r");
  if (fp != NULL) {
    char tmp[1024] = "";
    while (fgets(tmp, 1024, fp) != NULL) {
      fprintf(stdout, "%s", tmp);
    }
  }
  fclose(fp);
  fp = fopen("src/noFile.txt", "r");
  if (fp == NULL) {
    printf("!!!!!错误编号为：%d ", errno);
    printf("细则：%s\n", strerror(errno));
    // perror("错误：");
  } // 这里不应该fclose因为是空指针！！
  return 0;
}
