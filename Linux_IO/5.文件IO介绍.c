/*
● posix(可移植操作系统接口)定义的一组函数
● 不提供缓冲机制，每次读写操作都引起系统调用
● 核心概念是文件描述符
● 访问各类型文件
● Linux下，标准IO基于文件IO实现


  标准IO                	文件IO
  ANSI C标准(C89)       	POSIX标准(可移植操作系统接口)
  带缓冲                	无缓冲
  流FILE管理打开的文件  	通过文件描述符管理打开文件

*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int fuc_open();
void practice();

int main() {
  fuc_open();
  return 0;
}

/**
* @brief
● 成功时返回文件描述符，出错时返回EOF
● 打开文件时使用两个参数
● 创建文件时使用第三个参数指定新文件的权限
● 设备文件只能通过open打开，而不能通过open创建
*/
int fuc_open() {

  // 创建文件
  int fd;
  if ((fd = open("file/文件IO创建.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666)) <
      0) {
    perror("open");
    return -1;
  }

  // 读写
  if ((fd = open("file/文件IO创建.txt", O_RDWR | O_CREAT | O_EXCL, 06e66)) <
      0) {
    if (errno == EEXIST) {
      perror("文件存在");
    } else {
      perror("报错");
    }
  }

  // 关闭
  close(fd);
  return 0;
}

/**
 *@brief
 以读写方式打开文件test.txt，
 如果文件不存在则创建，如果存在则追加，返回描述符后，关闭描述符并结束*
 *
 *
 */
void practice() {
  int fd;
  if ((fd = open("file/test.txt", O_WRONLY | O_CREAT | O_APPEND, 0666)) < 0) {
    perror("未知错误");
  }
  close(fd);
}
