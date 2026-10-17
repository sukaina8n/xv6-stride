// stridetest.c: checks that the stride scheduler shares the CPU
// in proportion to tickets, and that settickets rejects bad input.
//
// Three children get 30, 20 and 10 tickets. Each one spins for
// 100 ticks and counts how many loop iterations it completed.
// More CPU time means a bigger count, so the counts should come
// out roughly 3 : 2 : 1. Run with: make qemu CPUS=1

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NCHILD 3
#define SPIN_TICKS 100

int
main(void)
{
  int tickets[NCHILD] = {30, 20, 10};

  // Failure case: fewer than 1 ticket must be rejected with -1.
  printf("settickets(0) = %d (expect -1)\n", settickets(0));

  for(int i = 0; i < NCHILD; i++){
    int pid = fork();
    if(pid < 0){
      printf("stridetest: fork failed\n");
      exit(1);
    }
    if(pid == 0){
      // Child: take our share of tickets, then spin and count.
      if(settickets(tickets[i]) != 0){
        printf("stridetest: settickets(%d) failed\n", tickets[i]);
        exit(1);
      }
      int start = uptime();
      long count = 0;
      while(uptime() - start < SPIN_TICKS)
        count++;
      printf("tickets=%d count=%d\n", tickets[i], (int)count);
      exit(0);
    }
  }

  // Parent: wait for all children to finish.
  for(int i = 0; i < NCHILD; i++)
    wait(0);

  exit(0);
}
