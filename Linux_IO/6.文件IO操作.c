#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
void fuc_read_and_write(int argc, char *argv[]);

/**
 *
 *@brieflinuxIO的操作
 * off_t  lseek(int fd, off_t offset, int whence);和fseek一致
 *
 *
 *
 */

int main(int argc, char *argv[]) {
  fuc_read_and_write(argc, argv);
  return 0;
}

void fuc_read_and_write(int argc, char *argv[]) {
  int fd, n, total = 0;
  char buff[1024] = {0};
  if (argc < 2) {
    perror("传入参数过少！");
    fprintf(stdout, "没有使用：%s <file> \n", argv[0]);
  }
  if ((fd = open(argv[1], O_RDONLY, 0666)) < 0) {
    perror("文件未找到");
    return;
  }

  // 读取数据
  while ((n = read(fd, buff, 1024))) {
    total += n;
    fprintf(stdout, "%s", buff);
  }
  fprintf(stdout, "文件大小为:  %d\n", total);
  close(fd);

  // 写入数据
  memset(buff, 0, 1024);
  if ((fd = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
    perror("创建失败：");
  }
  while ((fgets(buff, 1024, stdin)) != NULL) {
    if (strcmp(buff, "quit\n") == 0) {
      break;
    }
    write(fd, buff, 1024);
  }
  close(fd);
  return;
}
