#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

pid_t child_pids[2];
int num_children = 0;

void signal_handler(int sig) {
    int i;
    for (i = 0; i < num_children; i++) {
        if (child_pids[i] > 0) {
            kill(child_pids[i], SIGTERM);
        }
    }
    exit(1);
}

void child_process();
// parent process
int main (){
  int pid;
  int i;
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler); 
  for (i = 0; i<2; i++){
    pid_t pid = fork();
        if (pid == 0) {
            child_process();
        } else if (pid > 0) {
            child_pids[num_children++] = pid;
        } else {
            exit(1);
        }
  }

  for (i=0; i < 2; i++){
    int status;
    int completed_pid = wait(&status);
    printf("Child Pid: %d has completed with exit status: %d\n", completed_pid, WEXITSTATUS(status));
  }
  return 0;
}

// run one child process
void child_process(){
  pid_t pid = getpid();
  pid_t parent_pid = getppid();
  srandom(time(NULL) ^ (pid << 16));
  int n = 1 + (random() % 30); // random number no more than 30
  int i;
  for (i=0; i < n; i++){
    int time = 1 + (random() % 10); // sleep time no more than 10 seconds
    printf("Child Pid: %d is going to sleep for %d seconds!\n", pid, time);
    sleep(time); // sleep
    printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", pid, parent_pid); // print if awake
  }
  exit(0);
}


