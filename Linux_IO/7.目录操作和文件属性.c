#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
/**
 *opendir
 *readdir
 *closedir
 *
 */
void baseOprAboutDir(int argc, char *argv[]);
int main(int argc, char *argv[]) {
  baseOprAboutDir(argc, argv);
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
