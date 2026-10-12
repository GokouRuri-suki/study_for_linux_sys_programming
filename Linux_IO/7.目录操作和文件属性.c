#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
/**
 *opendir
 *readdir
 *closedir
 *
 */
void baseOprAboutDir(int argc, char *argv[]);
void baseOprAboutChmod(int agrc, char *argv[]);
void baseOprAboutFile(int argc, char *argv[]);
void baseOprAboutSt_mode(int argc, char *argv[]);
void practice(int argc, char *argv[]);
int main(int argc, char *argv[]) {
  // baseOprAboutDir(argc, argv);
  //  baseOprAboutFile(argc, argv);
  // baseOprAboutSt_mode(argc, argv);
  practice(argc, argv);
  return 0;
}
/**
 *  DIR * opendir(const char* name);
 *  struct dirent* readdir(DIR* dirp);
 *  int closedir(DIR* dirp);
 */
void baseOprAboutDir(int argc, char *argv[]) {
  if (argc < 2) {
    printf("参数缺少目标路径");
    return;
  }

  DIR *dirOpen = opendir(argv[1]);
  if (!dirOpen) {
    perror("没打开");
    return;
  }

  struct dirent *Dir;
  while ((Dir = readdir(dirOpen)) != NULL) {
    fprintf(stdout, "%s\n", Dir->d_name);
  }
  closedir(dirOpen);
  Dir = NULL;
}
void baseOprAboutChmod(int agrc, char *argv[]) {
  int fd;
  if ((fd = open(argv[1], O_WRONLY | O_CREAT, 0000)) < 0) {
    perror("分配失败");
  }
  // 改权限为0666
  // chmod(argv[1], 0666);
  // fchmod(fd, 06666);
}

/**
成员                      解释
mode_t    st_mode;      	类型和访问权限
uid_t     st_uid;       	所有者id
uid_t     st_gid;       	用户组id
off_t     st_size;      	文件大小
time_t    st_mtime;     	最后修改时间



//获取文件属性
int stat(const char* path, struct stat* buf);
int lstat(const char* path, struct stat* buf);
int fstat(int fd, struct stat* buf);

函数  	参数        	符号链接行为                      	典型场景
stat  	路径        	跟随链接，获取目标文件信息        	查普通文件属性
lstat 	路径        	不跟随链接，获取链接本身信息      	判断是否为软链接
fstat 	文件描述符  	同 stat，但用已打开的 fd          	已 open
后查元数据
*/
void baseOprAboutFile(int argc, char *argv[]) {
  if (argc < 2) {
    printf("参数缺少目标路径\n");
    return;
  }
  struct stat buf;
  if (stat(argv[1], &buf) == EOF) {
    perror("stat错误");
    return;
  }
  printf("<%s> 的stat的信息如下：\n", argv[1]);
  printf("mode_t  st_mode;   类型和访问权限: %o\n", buf.st_mode);
  printf("uid_t   st_uid;    所有者id:       %d\n", buf.st_uid);
  printf("uid_t   st_gid;    用户组id:       %d\n", buf.st_gid);
  printf("off_t   st_size;   文件大小:       %ld\n", (long)buf.st_size);
  printf("time_t  st_mtime;  最后修改时间:   %ld\n", (long)buf.st_mtime);
}
/*

 宏判断       	文件类型数值    	文件类型
S_ISREG(m)    	0100000         	判断是否是普通文件
S_ISDIR(m)    	0040000         	判断是否是目录
S_ISCHR(m)    	0020000         	判断是否是字符设备
S_ISBLK(m)    	0060000         	判断是否是块设备。
S_ISLNK(m)    	0120000         	判断是否是符号链接
S_ISFIFO(m)   	0010000         	判断是否是命名管道
S_ISSOCK(m)   	0140000         	判断是否是套接字


S_IRUSR 	用户读    	0400  	if (st_mode & S_IRUSR)    有读权限
S_IWUSR 	用户写    	0200  	if (st_mode & S_IWUSR)    有写权限
S_IXUSR 	用户执行  	0100  	if (st_mode & S_IXUSR)    有执行权限
也可以S_IRUSR(t.st_mode);
*/
void baseOprAboutSt_mode(int argc, char *argv[]) {
  struct stat buff;
  if (stat(argv[1], &buff) == EOF) {
    perror("错误");
    return;
  }
  printf("是否为普通文件:  %s", (S_ISREG(buff.st_mode) ? "YES\n" : "NO\n"));
  return;
}
/**

以下面格式打印指定文件的主要信息：
$./a.out test.c
-rw-r--r--  317  2014-11-08  test.c

提示
● 根据man 2 lstat查看函数所需头文件
● 用localtime时需要包含<time.h>
● 通过buf.st_mode & S_IFMT获取文件类型
● 通过buf.st_mode & 权限位获取权限

 */
void practice(int argc, char *argv[]) {
  if (argc < 2) {
    perror("缺少参数 <file> \n");
    return;
  }
  struct stat buf;
  int n;
  if (lstat(argv[1], &buf) == EOF) {
    perror("错误 stat获取失败");
    return;
  }
  printf("S_IFMT: %o \n", S_IFMT);
  // 以下为ls -l的实现关键部分处理文件夹和文件的前导d
  switch (buf.st_mode & S_IFMT) {
  case S_IFREG:
    printf("-");
    break;
  case S_IFDIR:
    printf("d");
  }
  for (int n = 8; n >= 0; n--) {
    if (buf.st_mode & (1 << n)) {
      switch (n % 3) {
      case 2:
        printf("r");
        break;
      case 1:
        printf("w");
        break;
      case 0:
        printf("x");
        break;
      }
    } else {
      printf("-");
    }
  }
  printf("\t\t\t\t文件大小为：%lu", buf.st_size);
  struct tm *tp = localtime(&buf.st_mtime);
  printf(" %d-%02d-%02d", tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday);
  printf("\t\t<%s>", argv[1]);
  return;
}
