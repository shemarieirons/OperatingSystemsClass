/* hello_timer.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int times=0;
time_t start_time, end_time;

void handler(int signum)
{ //signal handler
  printf("Hello World!\n");
  times+=1;
  alarm(5);
}

void sigint_handler(int signum)
{
  end_time = time(NULL);
  double total_time = difftime(end_time, start_time);

  printf("\nProgram executed for %.0f seconds (tracked %d alarms)\n",total_time, times);
  exit(0); // cleanly exit program
}



int main(int argc, char * argv[])
{
  start_time = time(NULL);
  signal(SIGALRM,handler); //register handler to handle SIGALRM
  signal(SIGINT, sigint_handler);
  
  alarm(5); //Schedule a SIGALRM for 5 seconds
  while(1) {
    pause();
    printf("Turing was right!\n");
  }  
  return 0; //never reached
}