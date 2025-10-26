/* hello_signal1.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int signalReceiver=0;

void handler(int signum)
{ //signal handler
  printf("Hello World!\n");
  signalReceiver=1;
}

int main(int argc, char * argv[])
{
  signal(SIGALRM,handler); //register handler to handle SIGALRM
  alarm(5); //Schedule a SIGALRM for 5 seconds
  while(signalReceiver==0) {
    printf("Turing was right!\n");
  }
  return 0; //never reached
}