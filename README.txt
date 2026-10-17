CS-333 Operating Systems, Fall 2026
Assignment 2: Stride Scheduler for xv6-riscv (Module 9)

Team
    Sukaina Nasir  (00000506084)  Person 1 
    Fatima Nisar   (00000504590)  Person 2
    Maryam Asif    (00000507587)  Person 3

Team repository: https://github.com/sukaina8n/xv6-stride  (branch: riscv)

1. What this is
xv6's round-robin scheduler is replaced with a stride scheduler.
Every process has tickets; more tickets means more CPU.
  stride = STRIDE1 / tickets      (STRIDE1 = 10000)
  pass   starts at 0; the scheduler runs the RUNNABLE process with the
         smallest pass, then adds that process's stride to its pass.
A new system call, settickets(int n), sets the caller's tickets
(returns 0 on success, -1 if n < 1). Default is 100 tickets.

Who did which part:
  Part 1 (Sukaina): STRIDE1, tickets/stride/pass in struct proc,
        defaults in allocproc(), inheritance in kfork(), the
        settickets syscall (number 23), user/settest.c
  Part 2 (Fatima):  scheduler() in kernel/proc.c (smallest-pass choice)
  Part 3 (Maryam):  user/stridetest.c, README, contribution statement,
        design note

2. Tools needed (Ubuntu)
  sudo apt update
  sudo apt install git build-essential gdb-multiarch qemu-system-misc \
       gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
(If qemu-system-riscv64 is not found on a newer Ubuntu:
  sudo apt install qemu-system-riscv)

3. Build and run
  make clean
  make qemu CPUS=1

CPUS=1 is used so the scheduling results are reproducible.
At the xv6 $ prompt run:
  settest
  stridetest
To quit QEMU press Ctrl-a, then x.

4. Test programs
settest (user/settest.c)  - tests the settickets system call only.
  Expected output:
    settickets(50) = 0 (expect 0)
    settickets(0)  = -1 (expect -1)
    settickets(-5) = -1 (expect -1)

stridetest (user/stridetest.c) - tests the scheduler.
  First the parent prints settickets(0), the failure case (expect -1).
  Then it forks three children with 30, 20 and 10 tickets. Each child
  spins for 100 ticks (measured with uptime()), counting loop
  iterations, and prints its tickets and count. Child output order can
  vary. The counts should be roughly 3 : 2 : 1.
  Example output (the exact counts differ on every run and machine):
    settickets(0) = -1 (expect -1)
    tickets=30 count=257663
    tickets=20 count=174858
    tickets=10 count=91696
  Here 257663 : 174858 : 91696 = 2.81 : 1.91 : 1, close to 3 : 2 : 1.

  Edge case that is NOT handled: settickets(n) with n > 10000 makes
  the stride 0 (see the design note, limitations).

5. Files in this submission
  xv6 source tree (after make clean)
  user/stridetest.c, user/settest.c   test programs
  README.txt                          this file
  CONTRIBUTIONS.txt                   contribution statement
  Design_Note.pdf                     design note

6. AI and online help used
  - xv6 book, Chapter 7 "Scheduling", and the original scheduler() in
    kernel/proc.c (https://github.com/mit-pdos/xv6-riscv)
  - C. A. Waldspurger and W. E. Weihl, "Stride Scheduling:
    Deterministic Proportional-Share Resource Management", MIT, 1995.
  - Claude (Anthropic): used by Fatima Nisar for guidance on the
    smallest-pass scheduler design, setup and git steps, and to help
    draft and format the Part 2 section of the design note.
  - Claude (Anthropic): used by Maryam Asif for guidance on the
    user/stridetest.c and assembling of combined design note.
  - Claude (Anthropic) : used by Sukaina for guidance on design of the settickets system call and new process fileds
