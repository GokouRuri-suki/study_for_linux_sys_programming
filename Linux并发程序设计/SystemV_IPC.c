#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
//
//
void ftokOpr();
void shmRD();
void shmLinkKey();
//
int main() {
  // ftokOpr();
  // shmLinkKey();
  shmRD();
  return 0;
};

//
//
void ftokOpr() {
  key_t k = ftok("file/ftokTest", 'A');
  if (k == (key_t)-1) {
    perror("ftok:");
    exit(EXIT_FAILURE);
  }
  // 没什么意义纯标识
  printf("key is %d\n", k);

  printf("key is %x\n", k);
  exit(EXIT_SUCCESS);
}

void shmLinkKey() {
  int shmid;
  key_t k;
  if ((k = ftok("./file/shmFile", 'A')) == -1) {
    perror("ftok:");
    exit(EXIT_FAILURE);
  }
  shmid = shmget(k, 1024, IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("shmget:");
    exit(EXIT_FAILURE);
  }
  printf("key:%d\n", k);
  printf("shm:%d\n", shmid);
  return;
};
void shmRD() {
  key_t k;
  int sid;
  char *addr;
  if ((k = ftok("file/shmFile", 'A')) == -1) {
    perror("key_t :");
    exit(EXIT_FAILURE);
  }
  if ((sid = shmget(k, 1024, IPC_CREAT | 0666)) == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }
  printf("shm:%d   key:%d", sid, k);
  if ((addr = shmat(sid, NULL, 0)) == (char *)-1) {
    perror("shmat:");
    exit(EXIT_FAILURE);
  }
  printf("请输入内容:\n");
  if ((fgets(addr, 1024, stdin)) == NULL) {
    perror("error:");
    exit(EXIT_FAILURE);
  }

  printf("内容为%s\n", addr);
  shmdt(addr);

  /* ┌────────┬──────────┬─────────────────────┬──────────────────────┐
   * │ 干什么 │ cmd      │ 实际效果            │ 什么时候用           │
   * ├────────┼──────────┼─────────────────────┼──────────────────────┤
   * │ 查     │ IPC_STAT │ 复制属性到 shmid_ds │ 多大/谁建的/几人在用 │
   * │ 改     │ IPC_SET  │ 写回 uid/gid/mode   │ 0666 收紧到 0600     │
   * │ 删     │ IPC_RMID │ 从内核摘除          │ 用完清理，否则残留   │
   * └────────┴──────────┴─────────────────────┴──────────────────────┘
   */
  shmctl(sid, IPC_RMID, NULL);
  return;
}
