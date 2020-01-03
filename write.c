#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 24605
#define SEMKEY 24604
struct sembuf sb;

int main(int argc, char const *argv[]) {
  printf("here1\n");
  int shmid = shmget(KEY, 4, 0644 | IPC_CREAT);
  if(errno){
    printf("Shared Memory error: %s\n", strerror(errno));
  }

  printf("trying to get in\n");

  int semid = semget(SEMKEY,1,0);
  printf("%d\n",semid);
  printf("here2\n");
  semop(semid, &sb, 1);
  printf("here3\n");

  char * data;
  data = shmat(shmid, 0, 0);
  printf("Last addition: %s\n", data);

  int file = open("story.txt", O_RDWR | O_APPEND, 0644);
  if(file == -1){
    printf("File error: %s\n", strerror(errno));
  }
  char text[1000];
  if(file == -1){
    printf("File error: %s\n", strerror(errno));
  }
  printf("Your addition: ");
  fgets(text,1000,stdin);
  printf("\n");

  write(file, text, strlen(text));
  close(file);

  shmdt(text);

  sb.sem_op = 1;
  semop(semid, &sb, 1);
  return 0;
}
