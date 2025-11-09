#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void ClientProcess(int []);

int main(int argc, char *argv[]) {
    int ShmID;
    int *ShmPTR;
    pid_t pid;
    int status;

    srand(time(NULL));  // Seed random number generator

    ShmID = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0666);
    if (ShmID < 0) {
        printf("*** shmget error ***\n");
        exit(1);
    }
    printf("Server has received a shared memory for two integers...\n");

    ShmPTR = (int *)shmat(ShmID, NULL, 0);
    if (ShmPTR == (void *)-1) {  // Fixed: Proper check for shmat failure (no cast needed)
        printf("*** shmat error ***\n");
        exit(1);
    }
    printf("Server has attached the shared memory...\n");

    ShmPTR[0] = 0;  // BankAccount
    ShmPTR[1] = 0;  // Turn
    printf("Server has initialized BankAccount to 0 and Turn to 0...\n");

    printf("Server is about to fork a child process...\n");
    pid = fork();
    if (pid < 0) {
        printf("*** fork error ***\n");
        exit(1);
    } else if (pid == 0) {
        ClientProcess(ShmPTR);
        exit(0);
    } else {
        // Parent process (Dear Old Dad) - Deposit loop 25 times
        for (int i = 0; i < 25; i++) {
            sleep(rand() % 6);  // Sleep 0-5 seconds
            int account = ShmPTR[0];
            while (ShmPTR[1] != 0);  // Busy wait for Turn == 0
            if (account <= 100) {
                int balance = rand() % 101;  // Random 0-100
                if (balance % 2 == 0) {
                    account += balance;
                    printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, account);
                } else {
                    printf("Dear old Dad: Doesn't have any money to give\n");
                }
            } else {
                printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
            }
            ShmPTR[0] = account;
            ShmPTR[1] = 1;  // Set Turn to 1
        }

        wait(&status);
        printf("Server has detected the completion of its child...\n");
        shmdt((void *)ShmPTR);
        printf("Server has detached its shared memory...\n");
        shmctl(ShmID, IPC_RMID, NULL);
        printf("Server has removed its shared memory...\n");
        printf("Server exits...\n");
        exit(0);
    }
}

void ClientProcess(int SharedMem[]) {
    // Child process (Poor Student) - Withdraw loop 25 times
    for (int i = 0; i < 25; i++) {
        sleep(rand() % 6);  // Sleep 0-5 seconds
        int account = SharedMem[0];
        while (SharedMem[1] != 1);  // Busy wait for Turn == 1
        int balance = rand() % 51;  // Random 0-50
        printf("Poor Student needs $%d\n", balance);
        if (balance <= account) {
            account -= balance;
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, account);
        } else {
            printf("Poor Student: Not Enough Cash ($%d)\n", account);
        }
        SharedMem[0] = account;
        SharedMem[1] = 0;  // Set Turn to 0
    }
}
