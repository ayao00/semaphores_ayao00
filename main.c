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


int main(int argc, char *argv[]){
  char * input;
  if(!argv[1]){
    printf("no argruments. Program exiting\n");
    return 0;
  }

  input = argv[1];
  int file;

//creating
  if(!strcmp(input, "-c")){
    int semaphore = semget(SEMKEY, 1, IPC_CREAT | 0644);
    if(semaphore == -1){
      printf("Semaphore Error: %s\n", strerror(errno));
      semaphore = semget(SEMKEY, 1, 0);
      int v = semctl(semaphore, 0, GETVAL, 0);
    }else{
      union semun data;
      data.val = 1;
      int r = semctl(semaphore, 0, SETVAL, data);
    }
    printf("semaphore created\n");
    int shmid = shmget(KEY,  sizeof(char *), 0644 | IPC_CREAT);
    if(shmid == -1){
      printf("Shared Memory error: %s\n", strerror(errno));
    }else{
      printf("shared memory created\n");
    }
    file = open("./story.txt", O_CREAT | O_TRUNC, 0644);
    if(file == -1){
      printf("File error: %s\n", strerror(errno));
    }
    printf("file created\n");
  }
//viewing
  else if(!strcmp(input, "-v")){
    file = open("./story.txt", O_RDONLY);
    if(file == -1){
      printf("File error: %s\n", strerror(errno));
    }
    char readin[1000];
    read(file, readin,999);
    readin[999] = '\0';
    close(file);
    printf("The story so far:\n%s\n",readin);
  }
//removing
  else if(!strcmp(input, "-r")){
    int shmid = shmget(KEY, 4, 0644);
    if(errno){
      printf("Shared Memory error: %s\n", strerror(errno));
    }
    printf("trying to get in\n");

    char story[1000];
    file = open("./story.txt", O_RDONLY);
    read(file, story, 999);
    close(file);
    printf("The story so far:\n%s\n",story);

    shmctl(shmid, IPC_RMID, NULL);
    printf("shared memory deleted\n");

    remove("story.txt");
    printf("file deleted\n");

    int semaphore = semget(SEMKEY, 1, IPC_CREAT | 0644);
    union semun data;
    semctl(semaphore, 1, IPC_RMID, data);
    printf("semaphore deleted\n");
  }
  else{
    printf("Error: Please input one of the following flags(-c, -v, -r)\n");
  }
  return 0;
}
