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
  int shmid = shmget(KEY, 4, 0644 | IPC_CREAT);
  if(errno){
    printf("Shared Memory error: %s\n", strerror(errno));
  }

  printf("trying to get in\n");

  int semid = semget(SEMKEY,1,0);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  semop(semid, &sb, 1);

  int * data;
  printf("Last addition: ");
  data = shmat(shmid, (void *)0, 0);

  int file = open("./story.txt", O_RDONLY);
  if(file == -1){
    printf("File error: %s\n", strerror(errno));
  }
  lseek(file, 0 - *data, SEEK_END);
  char lastline[256];
  read(file, lastline, *data);
  close(file);
  printf("%s\n", lastline);


  char text[1000];
  file = open("./story.txt", O_WRONLY | O_APPEND);
  if(file == -1){
    printf("File error: %s\n", strerror(errno));
  }
  printf("Your addition: ");
  fgets(text,1000,stdin);
  printf("\n");
  *data = (int)strlen(text);

  write(file, text, strlen(text));
  close(file);

  shmid = shmget( KEY, 4, 0644|IPC_CREAT);

  sb.sem_op = 1;
  semop(semid, &sb, 1);
  return 0;
}
