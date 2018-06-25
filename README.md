# 1. Operating Systems Exam

Hopefully all question types, the theory and examples.

## 1.1. Table of Contents

<!-- TOC -->

- [1. Operating Systems Exam](#1-operating-systems-exam)
    - [1.1. Table of Contents](#11-table-of-contents)
    - [1.2. Scheduling](#12-scheduling)
        - [1.2.1. Measuring Scheduling Effectiveness](#121-measuring-scheduling-effectiveness)
        - [1.2.2. FCFS](#122-fcfs)
            - [1.2.2.1. Example](#1221-example)
        - [1.2.3. SPN & SRTN(SPN w/ preemtion)](#123-spn--srtnspn-w-preemtion)
            - [1.2.3.1. SPN](#1231-spn)
            - [1.2.3.2. SRTN](#1232-srtn)
            - [1.2.3.3. Approximating Process Runtime](#1233-approximating-process-runtime)
        - [1.2.4. Highest Priority First (HPF) w/ & wo/ preemtion](#124-highest-priority-first-hpf-w--wo-preemtion)
            - [1.2.4.1. Without preemption](#1241-without-preemption)
            - [1.2.4.2. With preemption](#1242-with-preemption)
        - [1.2.5. Round Robin](#125-round-robin)
            - [1.2.5.1. quantum = 3](#1251-quantum--3)
            - [1.2.5.2. quantum = 1](#1252-quantum--1)
        - [1.2.6. Lottery Scheduling(w/ compensation tickets)](#126-lottery-schedulingw-compensation-tickets)
            - [1.2.6.1. Example](#1261-example)
        - [1.2.7. Old Exam Questions](#127-old-exam-questions)
            - [1.2.7.1. Example 1](#1271-example-1)
            - [1.2.7.2. Example 2](#1272-example-2)
    - [1.3. Processes and Threads](#13-processes-and-threads)
        - [1.3.1. User vs Kernel Threads](#131-user-vs-kernel-threads)
    - [1.4. IPC](#14-ipc)
        - [1.4.1. Pipes](#141-pipes)
            - [1.4.1.1. Unnamed Pipes](#1411-unnamed-pipes)
            - [1.4.1.2. Named Pipes(FIFOs)](#1412-named-pipesfifos)
        - [1.4.2. Shared Memory & Message Queues](#142-shared-memory--message-queues)
            - [1.4.2.1. Shared Memory](#1421-shared-memory)
            - [1.4.2.2. Message Queues](#1422-message-queues)
        - [1.4.3. Semaphores](#143-semaphores)
            - [1.4.3.1. Example Task](#1431-example-task)
    - [1.5. Sync](#15-sync)
        - [1.5.1. Dekkers Algorithm (Software)](#151-dekkers-algorithm-software)
        - [1.5.2. Bakery Algorithm (Software)](#152-bakery-algorithm-software)
        - [1.5.3. Semaphores (OS)](#153-semaphores-os)
        - [1.5.4. Monitor(Cond. Variables with Mutexes(Semaphores)) (OS)](#154-monitorcond-variables-with-mutexessemaphores-os)
        - [1.5.5. Interrupt Disabling (HW)](#155-interrupt-disabling-hw)
        - [1.5.6. Test and Set (HW)](#156-test-and-set-hw)
    - [1.6. Deadlocks](#16-deadlocks)
        - [1.6.1. Banker Algorithm](#161-banker-algorithm)
            - [1.6.1.1. Example](#1611-example)
            - [1.6.1.2. Example with Requests](#1612-example-with-requests)
                - [1.6.1.2.1. Example 1](#16121-example-1)
                - [1.6.1.2.2. Example 2](#16122-example-2)
            - [1.6.1.3. Detecting Deadlocks](#1613-detecting-deadlocks)
            - [1.6.1.4. Deadlock Handling](#1614-deadlock-handling)
        - [1.6.2. Old Exam Questions](#162-old-exam-questions)
            - [1.6.2.1. Example 1](#1621-example-1)
            - [1.6.2.2. Example 2](#1622-example-2)
    - [1.7. Virtual Memory & Memory Allocation](#17-virtual-memory--memory-allocation)
        - [1.7.1. Types of realization of virtual Memory](#171-types-of-realization-of-virtual-memory)
            - [1.7.1.1. Base & Bound and Segmentation](#1711-base--bound-and-segmentation)
            - [1.7.1.2. Paging](#1712-paging)
                - [1.7.1.2.1. Working Set Strategy](#17121-working-set-strategy)
                - [1.7.1.2.2. OPT (Optimal)](#17122-opt-optimal)
                - [1.7.1.2.3. FIFO](#17123-fifo)
                - [1.7.1.2.4. LRU (least recently used)](#17124-lru-least-recently-used)
                - [1.7.1.2.5. Clock Policy](#17125-clock-policy)
        - [1.7.2. Allocators and Garbage Collectors](#172-allocators-and-garbage-collectors)
            - [1.7.2.1. Best Fit Allocator](#1721-best-fit-allocator)
            - [1.7.2.2. First Fit Allocator (LIFO vs FIFO)](#1722-first-fit-allocator-lifo-vs-fifo)
            - [1.7.2.3. Slab Allocator](#1723-slab-allocator)
            - [1.7.2.4. Free List Allocator](#1724-free-list-allocator)
            - [1.7.2.5. Stop and Copy Garbage Collection](#1725-stop-and-copy-garbage-collection)
            - [1.7.2.6. Concurrent Garbage Collection](#1726-concurrent-garbage-collection)
            - [1.7.2.7. Reference Counting Garbage Collection](#1727-reference-counting-garbage-collection)
    - [1.8. I/O](#18-io)
        - [1.8.1. Memory Mapping](#181-memory-mapping)
        - [1.8.2. DMA (Direct Memory Access)](#182-dma-direct-memory-access)
        - [1.8.3. HDDs](#183-hdds)
            - [1.8.3.1. FCFS](#1831-fcfs)
                - [1.8.3.1.1. Example](#18311-example)
            - [1.8.3.2. SPTF(Shortest Positioning Time First)](#1832-sptfshortest-positioning-time-first)
                - [1.8.3.2.1. Example](#18321-example)
            - [1.8.3.3. Aged SPTF(Shortest Positioning Time First)](#1833-aged-sptfshortest-positioning-time-first)
            - [1.8.3.4. SCAN, CSCAN & VSCAN](#1834-scan-cscan--vscan)
                - [1.8.3.4.1. Example SCAN](#18341-example-scan)
                - [1.8.3.4.2. Example CSCAN](#18342-example-cscan)
    - [1.9. Multiprocessor & Multicomputer](#19-multiprocessor--multicomputer)
        - [1.9.1. Multiprocessor](#191-multiprocessor)
            - [1.9.1.1. Uniform Memory Access(UMA)](#1911-uniform-memory-accessuma)
            - [1.9.1.2. NUMA (Non-UMA)](#1912-numa-non-uma)
            - [1.9.1.3. Chaches](#1913-chaches)
                - [1.9.1.3.1. Cache coherence](#19131-cache-coherence)
                - [1.9.1.3.2. Write Back](#19132-write-back)
                - [1.9.1.3.3. Write Through](#19133-write-through)
            - [1.9.1.4. Scheduling of Multiprocessor Systems](#1914-scheduling-of-multiprocessor-systems)
        - [1.9.2. Multicomputer](#192-multicomputer)
            - [1.9.2.1. Blocking vs. Nonblocking](#1921-blocking-vs-nonblocking)
            - [1.9.2.2. Distributed Shared Memory(DSM)](#1922-distributed-shared-memorydsm)
    - [1.10. Virtualization](#110-virtualization)
        - [1.10.1. Hypervisors/VMM (Virtual Machine Monitor)](#1101-hypervisorsvmm-virtual-machine-monitor)
        - [1.10.2. Automation](#1102-automation)
        - [1.10.3. Dynamic Resource Management](#1103-dynamic-resource-management)
        - [1.10.4. Elastic I/O](#1104-elastic-io)
    - [1.11. References](#111-references)

<!-- /TOC -->

## 1.2. Scheduling

Scheduling is the problem of assigning a process to the CPU. Since later arriving processes, the process runtime and other factors are unknown, this is a NP problem.

### 1.2.1. Measuring Scheduling Effectiveness

| Name                      | Description                                            |
|---------------------------|--------------------------------------------------------|
| **Throughput**            | Total number of processes / total time to complete all |
| **Turnaround Time**       | The total time for a process (waiting + CPU)           |
| **Response Time/Latency** | Time waited for first CPU time                         |
| **Waiting Time**          | Total time a process needs to wait until completed     |
| **Fairness/Starvation**   | Does every process necessarily finish?                 |

Naturally all of the measurements can be averaged.

### 1.2.2. FCFS

First arriving process gets executed first.

#### 1.2.2.1. Example

**Note:** SPN is used if two processes arrive at the same time.

| Process | Arrival Time | Service Time |
|---------|--------------|--------------|
| A       | 0            | 3            |
| B       | 0            | 1            |
| C       | 1            | 3            |
| D       | 2            | 3            |

Solution:

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|
| B | A | A | A | C | C | C | D | D | D |

### 1.2.3. SPN & SRTN(SPN w/ preemtion)

Shortest Process Next(SPN) schedules the waiting process with least running time next and
Shortest remaining time next is SPN but stops the currently executing process if a shorter one appears.

#### 1.2.3.1. SPN

| Process | Arrival Time | Service Time |
|---------|--------------|--------------|
| A       | 0            | 3            |
| B       | 0            | 1            |
| C       | 1            | 3            |
| D       | 2            | 3            |

Solution:

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|
| B | A | A | A | C | C | C | D | D | D |

#### 1.2.3.2. SRTN

| Process | Arrival Time | Service Time |
|---------|--------------|--------------|
| A       | 0            | 4            |
| B       | 0            | 1            |
| C       | 1            | 3            |
| D       | 2            | 1            |

Solution:

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|
| B | C | D | C | C | A | A | A | A |

#### 1.2.3.3. Approximating Process Runtime

There are two strategies to approximating a processes runtime:

- **Exponential Averaging:** The idea is that the latest times have more relevance than those further in the past. The newer times get exponentially more weight in the average and the older ones exponentially less. This leads to faster adaption to bigger changes in process times. Yet single statistical outliers may get to much attention.
- **Averaging:** All measured times divided by the number of measurements. Adjusts slower to changes but is not really effected by statistical outliers.

### 1.2.4. Highest Priority First (HPF) w/ & wo/ preemtion

Each process has an assigned priority. The process with the highest priority gets executed first.
For UNIX systems a lower number normally equals higher priority.

#### 1.2.4.1. Without preemption

| Process | Arrival Time | Service Time | Priority |
|---------|--------------|--------------|----------|
| A       | 0            | 3            | 3        |
| B       | 0            | 1            | 1        |
| C       | 2            | 3            | 2        |
| D       | 3            | 2            | 1        |

Solution:

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|
| B | A | A | A | D | D | C | C | C |

#### 1.2.4.2. With preemption

| Process | Arrival Time | Service Time | Priority |
|---------|--------------|--------------|----------|
| A       | 0            | 3            | 3        |
| B       | 0            | 1            | 1        |
| C       | 2            | 3            | 2        |
| D       | 3            | 2            | 1        |

Solution:

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|
| B | A | C | D | D | C | C | A | A |

### 1.2.5. Round Robin

Each waiting process gets queued in the waiting queue. After a short amount of time (quantum) the currently running process gets enqueued and the next executing process is the first one in the queue.

With a big quantum round robin will become FCFS scheduling, with a to short quantum the context switch overhead will make it useless.

#### 1.2.5.1. quantum = 3

**Note:** SPN is used when two processes arrive at the same time.

| Process | Arrival Time | Service Time |
|---------|--------------|--------------|
| A       | 0            | 3            |
| B       | 0            | 1            |
| C       | 2            | 3            |
| D       | 3            | 2            |

Solution:

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|
| B | A | A | A | C | C | C | D | D |

#### 1.2.5.2. quantum = 1

**Note:** SPN is used when two processes arrive at the same time.

| Process | Arrival Time | Service Time |
|---------|--------------|--------------|
| A       | 0            | 3            |
| B       | 0            | 1            |
| C       | 2            | 3            |
| D       | 3            | 2            |

Solution:

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|
| B | A | C | A | D | C | A | D | C |

At time 3 D gets enqued before C.

### 1.2.6. Lottery Scheduling(w/ compensation tickets)

This example illustrates the behavior of __lottery scheduling using compensation tickets__.

#### 1.2.6.1. Example

Consider the following input data set of four processes arriving at the given arrival time in the ready queue of the scheduler:

| Process | Arrival Time | Service Time | Tickets  |
|---------|--------------|--------------|----------|
| A       | 0            | 3            | 8        |
| B       | 0            | 1            | 6        |
| C       | 1            | 3            | 17       |
| D       | 2            | 3            | 10       |

The processes only require the CPU and are not going to be interrupted by I/O or any other reasons. The Scheduler only decides *online*, i.e. only based on the information of currently present processes. The quantum is __2__ time units. Processes __A__ and __C__ only use __1__ time unit of their quantum, while all other processes use the __full__ quantum.

Specify for every point in time from 0 to 9 (inclusive) the probability with which a process is scheduled to use the CPU. Assume that - for every drawing - always the process with the highest probability wins.


| Time | %A    | %B   | %C    | %D    | Choice | Notes                                  |
|------|-------|------|-------|-------|--------|----------------------------------------|
| 0    | 8/14  | 6/14 |       |       | A      |                                        |
| 1    | 16/39 | 6/39 | 17/39 |       | C      | A yields, gets 8  compensation tickets |
| 2    | 16/66 | 6/66 | 34/66 | 10/66 | C      | C yields, gets 17 compensation tickets |
| 3    | 16/66 | 6/66 | 34/66 | 10/66 | C      | C finishes now                         |
| 4    | 16/32 | 6/32 |       | 10/32 | A      | A yields, gets 8 compensation tickets  |
| 5    | 16/32 | 6/32 |       | 10/32 | A      | A finishes now                         |
| 6    |       | 6/16 |       | 10/16 | D      |                                        |
| 7    |       | 6/16 |       | 10/16 | D      |                                        |
| 8    |       | 6/16 |       | 10/16 | D      | D finished now                         |
| 9    |       | 6/6  |       |       | B      | B finished now                         |

At time 1 A yields the CPU although the quantum is 2. For this reason, it is compensated by doubling its tickets (factor 2 computed via (available time quantum) / (actual time used), lecture part 4, slide 41).

At time 2 C yields the CPU and is compensated likewise. However, as it is immediately re-scheduled to use the CPU, its compensation tickets are removed again (lecture part 4, slide 42). C again yields after 1 time unit and is hence again compensated.

Note that the originally assigned tickets never expire. They are only removed once a process finishes its execution. Only compensation tickets expire whenever a process gets the CPU.

### 1.2.7. Old Exam Questions

#### 1.2.7.1. Example 1

Explain the method of approximating a processes runtime with exponential averaging. Are there situations where simply averaging is better?

**Exponential Averaging:** The latest measurements get exponentially more weight in the average than the older measurements. This allows to ajust to bigger changes in the runtime of a program fast.

A big outlier may affect exponential averaging a lot more than simple averaging.

If a program was run `100` times and the latest measurement were `100` and all the older measurements were below `20`, exponential averaging would probably approximate the running time of the programm above `50` whereas the normal average would be less than `25`, which is a lot better approximation if it was an one time outlier.

#### 1.2.7.2. Example 2

The average process runs for `T` time, before it blocks due to I/O. A context switch needs the time `S`(Overhead). Give a formula for the CPU efficiency if round robin with quantum `Q` is used(CPU Efficiency = useful CPU Time / total CPU time) if:

- `Q >= T`
- `S < Q < T`
- `Q = S`

Solution:

- `Q >= T`

| Useful Time | Wasted Time | Total Time |
| --- | --- | --- |
| `Q - (Q - T) = T` | `S` | `T + S` |

Efficiency = `T / (T + S)`

- `S < Q < T`

| Useful Time | Wasted Time | Total Time |
| --- | --- | --- |
| `T / Q * Q = T` | `T / Q * S` | `T + T / Q * S` |

**Note:** The useful time is the average time a process is working, the wasted time is the average number of context switches times the time per context switch for the average time.

Efficiency = `T / (T + T / Q * S)` (with `T / Q > 1`)

- `Q = S`

| Useful Time | Wasted Time | Total Time |
| --- | --- | --- |
| `T / Q * Q = T` | `T / Q * S = T / S * S = T` | `T + T` |

Efficiency = `T / (T + T) = T / 2T = 1 / 2`

## 1.3. Processes and Threads

### 1.3.1. User vs Kernel Threads

| User Threads | Kernel/System Thread |
| --- | --- |
| The system only sees one thread, thread switching is achieved without the OS. I/O becomes easier. No parallelism on mulitprocessor-architecture. | The OS sees every thread of a process. Achieves parallelism on mulitprocessor-architecture. |

## 1.4. IPC

### 1.4.1. Pipes

#### 1.4.1.1. Unnamed Pipes

Pipe has write and read end(data only flows in one direction(half-duplex)) and is nonblocking. Only works **within same process group!**

**Example:** Unnamed pipe used to implement/emulate the bash pipe.

```C
#include <unistd.h>

#define READ 0
#define WRITE 1

...

int pipefd[2];
pipe(pipefd);

...

// e.g child:
close(pipefd[READ]);

dup2(pipefd[WRITE], stdout)
close(pipefd[WRITE]);

// write to pipe

// terminate child

// e.g parent
close(pipefd[WRITE]);

dup2(pipefd[READ], stdin)
close(pipefd[READ]);

// read from pipe

...

// exit
```

#### 1.4.1.2. Named Pipes(FIFOs)

Pipe which is implemented as pseudofile and can be accessed by any process.

**Example:** Duplicates the output of process A to program B and C (without writing to an intermediate file).

Note: `tee` copies its `stdin` to `stdout` and a file(each FIFO is a pseudofile).

- make a fifo `mkfifo(name)`
- let C read from the fifo
- direct the output from A to `tee`
- redirect `tee`s output to B and to the fifo

Other use case: A server has one input fifo to accept requests and opens one answer fifo for each request.

### 1.4.2. Shared Memory & Message Queues

See lecture notes, probably not relevant.

#### 1.4.2.1. Shared Memory

```C
// get shared memory
int shmid;
data_t *data;
shmid = shmget(SIZE);
data = shmat(shmid);
// use data to modify/read the data_t data
shmctl(shmid, IPC_RMID);
shmdt(shmid);
```

#### 1.4.2.2. Message Queues

```C
// get messagequeue
int messageQueueID = msgget(KEY);

// send message
msgsnd(messageQueueID, message)

// receive message
msgrcv(messageQueueID, buffer)
```

### 1.4.3. Semaphores

Semaphores are global flags in shared memory. If the semaphore value is >0, processes can acquire(P(sem)) the semaphore until the value is 0. Each acquisition(P(sem)) decreases the semaphore value by one. If a process tries to acquire(P(sem)) a semaphore with value 0 it will wait until the value is >0. After the critical region a post(V(sem)) operation increases the value by 1 allowing other processes to enter the critical region.

**Note:** Deadlocks may be possible if a process dies within a critical region(no other process may be able to get into the critical region).

**Note:** Using **V(sem)** for **post** and **P(sem)** for **wait** is stupid but used in the lecture.

#### 1.4.3.1. Example Task

Each of the processes A, B and C is started at the same time and the instructions are executed exactly once and then the process terminates. Find all possible outputs(all processes write to the same output). Write each possible sequence in a separate line.

The four semaphore variables `s1`, `s2`, `s3`and `s4` are initialized to `0`, `0`, `1` and `0` respectively.

| Process A     | Process B     | Process C     |
| ---           | ---           | ---           |
| `P(s1)`       | `P(s3)`       | `P(s2)`       |
| `write('C')`  | `write('B')`  | `P(s1)`       |
| `V(s2)`       | `V(s1)`       | `write('A')`  |
| `V(s1)`       | `P(s4)`       | `V(s3)`       |
| `P(s3)`       | `P(s1)`       | `V(s1)`       |
| `P(s1)`       | `write('D')`  |               |
| `write('E')`  | `V(s1)`       |               |
| `V(s1)`       |               |               |
| `V(s4)`       |               |               |

Solution:

Since only `s3` can be acquired, B must begin. Only A can continue and acquires `s1`. Then only C can continue since `s3` and `s4` are `0` and C cann access all needed semaphores. Then only A can continue and finally B finishes. The final semaphore values are `1`, `0`, `0` and `0` for `s1`, `s2`, `s3`and `s4` respectively.

Output:

`BCAED`

No other output is possible

## 1.5. Sync

>A race condition is the behavior of a program where the output is dependent on uncontrollable events. It becomes a bug when events do not happen in the order the programmer intended.

Example: Two processes share a critical region where the access is controlled by an integer type named `counter`.

| Process A               | Process B                                          |
|-------------------------|----------------------------------------------------|
| `while(counter == n);`  | `while(counter == 0);`                             |
| `// critical region`    | `// critical region`                               |
| `counter++`             | `counter--`                                        |

Each in/decrement of counter internally is realized by the process first getting counter, changing its value and then writing it back into the shared memory. Since `counter++/--` is not atomic (does need more than one CPU cycle) data inconsitencies can appear.

Proper syncronisation avoids race conditions either using a software, OS or hardware solution.

### 1.5.1. Dekkers Algorithm (Software)

Dekkers algorithm achieves mutual exclusion in a critical region for 2 processes A and B, by using a global flag wheter the other process wants to enter the critical region and a turn.

```C
variables
        wants_to_enter : array of 2 booleans
        turn : integer

    wants_to_enter[A] ← false
    wants_to_enter[B] ← false
    turn ← A   // or B
```

The code for A:

```C
boolean wants_to_enter[2];
int turn;

for(;;){
  ...
  wants_to_enter[A] = true;

  turn = B;
  // wait until B finished its turn
      // or doesn't want the region
  while(wants_to_enter[B] && turn == B);


  // critical region
  wants_to_enter[A] = false;
  turn = B;
  ...
}
```

**Note:** Am i not seeing something or is the other process definitely stuck if the other dies in the critical region?

### 1.5.2. Bakery Algorithm (Software)

The bakey algorithm is a software mutual exclusion solution for `N` processes.

1. Each process which wants to enter the critical region gets a ticket. The process with the smallest ticket enters the critical region first.
2. If two processes `pi` and `pj` have the same ticket number, and `i < j`, then `pi` enters first.
3. After the critical region the ticket of a process is set to `0`.

```C
variables
        choosing : array of N booleans
        tickets  : array of N integers

    for all 0 <= i <N:
    choosing[i] ← false;
    ticket[i]   ← 0;
```

The code for process `pi`:

```C
for(;;){
    ...
    // get a ticket
    choosing[i] = true;
    ticket[i] = max(ticket) + 1; // max of all tickets
    choosing[i] = false;

    // wait until ticket is next
    for(j = 0 to N-1){
        // if j gets a ticket wait
        while(choosing[j]);
        // compare tickets
        while(ticket[j] != 0 and (ticket[j], j) < (ticket[i], i));
    }

    // critical region
    ticket[i] = 0;
    ...
}
```

Choosing is necessary, because if two processes `pi` and `pj` with `j < i` supposedly want to enter the critical region and both get the same `ticket`, if `pj` was preempted before setting `tickets[j]` then for `pi` the `ticket` of `pj` is `0` and it will therefore enter the critical region. Later `pj` will also enter the critical region because `ticket[i] == ticket[j]` and `j < i`.

### 1.5.3. Semaphores (OS)

See [IPC](#133-semaphores)

### 1.5.4. Monitor(Cond. Variables with Mutexes(Semaphores)) (OS)

A monitor is a synchronization construct that allows threads to have both mutual exclusion and the ability to wait (block) for a certain condition to become true. Monitors also have a mechanism for signaling other threads that their condition has been met.

Basically a semaphore(mutex) combined with a condition variable. The benefit is a more detailed control over specific conditions, for little overhead more.

**Example:** Suppose a producer writes in a queue and multiple consumers want to access the elements. Mutex is used to only allow read or write operations and a condition variable is used to distinguish between an empty or non-empty queue. This allows the consumers to only try to acquire the queue if there are elements to consume.

### 1.5.5. Interrupt Disabling (HW)

On a **single**-core CPU a process can simply deactivate interrupts(preemption) before entering the critical region and reactivate them after the critical region.

### 1.5.6. Test and Set (HW)

Basically **Test** and **Test and Set** is making special machine instructions for atomically accessing and manipulate a semaphore.

Test just accesses the semaphore and returns wheter it is free to enter.

Test and Set accesses the semaphore, locks it if it wasn't and  returns wheter it was acquired or if it is locked by another process.

**Problem:** Assume a low-priority process A acquires the lock, gets preempted for the high-priority process B which tries to lock the semaphore but can't since A has it. A won't get CPU time since it has the lower priority.

## 1.6. Deadlocks

The four requirements for a deadlock:

1. **Mutual Exclusion:** Every resource can only be allocated to one process(e.g no multiple readers).
2. **Hodl and Wait:** A process can acquire a resource and wait for others.
3. **No voluntary deallocation of resources:** No process voluntarily gives up its allocated resources.
4. **Circular Wait:** At least two processes wait for the resources the other one has. Is a cycle in a Resource Allocation Graph(RAG).

1.-3. are necessary for a deadlock to be possible, but only if all four occur a deadlock is given. If only one does not apply, there can be no deadlock.

### 1.6.1. Banker Algorithm

Used to find a safe(deadlock free) order of executing a programm.

1. For all processes their currently allocated, still needed and the total number of available resources is needed.
2. For each process in the list look if there are enough available resources left, if yes execute this process now and get the processes resources back and add it to the safe path
3. If either all processes are done or there is no process which can be finished, the algorithm stops. If there are still unfinished processes left the state is unsafe, otherwise a safe path was found.

The total number or resources is the sum of all allocated resources + Available.

#### 1.6.1.1. Example

Initial State:

|   | Allocated   | Need        | Max         | Available   |
|---|-------------|-------------|-------------|-------------|
|   |   P  Q  S   |   P  Q  S   |   P  Q  S   |   P  Q  S   |
| A |   0  1  2   |   2  1  1   |   2  2  3   |   1  1  1   |
| B |   2  0  0   |   1  1  0   |   3  1  0   |             |
| C |   1  2  1   |   3  2  1   |   4  4  2   |             |
| D |   0  1  1   |   4  4  4   |   4  5  5   |             |

Safe sequence: `<>`

1. Can A be fullfilled?
    1. NO
2. Can B fullfilled?
    1. YES
    2. update Available and safe sequence

|     | Allocated   | Need        | Max         | Available   |
| --- |-------------|-------------|-------------|-------------|
|     |   P  Q  S   |   P  Q  S   |   P  Q  S   |   P  Q  S   |
|  A  |   0  1  2   |   2  1  1   |   2  2  3   | ~~1  1  1~~ |
|~~B~~|   2  0  0   |   1  1  0   |   3  1  0   |   3  1  1   |
|  C  |   1  2  1   |   3  2  1   |   4  4  2   |             |
|  D  |   0  1  1   |   4  4  4   |   4  5  5   |             |

Safe sequence: `<B>`

3. Can C be fullfilled?
    1. NO
4. Can D be fullfilled?
    1. NO
5. Can A be fullfilled?
    1. YES
    2. update Available and safe sequence

|     | Allocated   | Need        | Max         | Available   |
| --- |-------------|-------------|-------------|-------------|
|     |   P  Q  S   |   P  Q  S   |   P  Q  S   |   P  Q  S   |
|~~A~~|   0  1  2   |   2  1  1   |   2  2  3   | ~~1  1  1~~ |
|~~B~~|   2  0  0   |   1  1  0   |   3  1  0   | ~~3  1  1~~ |
|  C  |   1  2  1   |   3  2  1   |   4  4  2   |   3  2  3   |
|  D  |   0  1  1   |   4  4  4   |   4  5  5   |             |

Safe sequence: `<B, A>`

6. Can C be fullfilled?
    1. YES
    2. update Available and safe sequence

|     | Allocated   | Need        | Max         | Available   |
| --- |-------------|-------------|-------------|-------------|
|     |   P  Q  S   |   P  Q  S   |   P  Q  S   |   P  Q  S   |
|~~A~~|   0  1  2   |   2  1  1   |   2  2  3   | ~~1  1  1~~ |
|~~B~~|   2  0  0   |   1  1  0   |   3  1  0   | ~~3  1  1~~ |
|~~C~~|   1  2  1   |   3  2  1   |   4  4  2   | ~~3  2  3~~ |
|  D  |   0  1  1   |   4  4  4   |   4  5  5   |   4  4  4   |

Safe sequence: `<B, A, C>`

7. Can D be fullfilled?
    1. YES
    2. update Available and safe sequence

|     | Allocated   | Need        | Max         | Available   |
| --- |-------------|-------------|-------------|-------------|
|     |   P  Q  S   |   P  Q  S   |   P  Q  S   |   P  Q  S   |
|~~A~~|   0  1  2   |   2  1  1   |   2  2  3   | ~~1  1  1~~ |
|~~B~~|   2  0  0   |   1  1  0   |   3  1  0   | ~~3  1  1~~ |
|~~C~~|   1  2  1   |   3  2  1   |   4  4  2   | ~~3  2  3~~ |
|~~D~~|   0  1  1   |   4  4  4   |   4  5  5   | ~~4  4  4~~ |

Safe sequence: `<B, A, C, D>`

Since all processes have been completed, the algorithm finishes and a safe sequence exists.

#### 1.6.1.2. Example with Requests

If a process requests a resource

1. first it needs to be checked if the request is smaller than the processes need(Need/Max would be false otherwise)
2. then it needs to be checked wheter there are enough resources available,
3. if 1. and 2. return true, update Allocated, Need and Available and then use Bankers Algorithm to determine if there exists a safe sequence.
4. Only grant a request if a safe sequence exists.

Initial State:

|   | Allocated   | Need        | Max         | Available   |
|---|-------------|-------------|-------------|-------------|
|   |   P  Q  S   |   P  Q  S   |   P  Q  S   |   P  Q  S   |
| A |   0  1  2   |   2  1  1   |   2  2  3   |   1  1  1   |
| B |   2  0  0   |   1  1  0   |   3  1  0   |             |
| C |   1  2  1   |   3  2  1   |   4  4  2   |             |
| D |   0  1  1   |   4  4  4   |   4  5  5   |             |

##### 1.6.1.2.1. Example 1

Process A requests `1` unit of resource `P`.

1. Is the request smaller than need?
    1. YES
2. Is the request smaller than available?
    1. YES
    2. update Allocated and Need of A and Available

|   | Allocated   | Need        | Max         | Available   |
|---|-------------|-------------|-------------|-------------|
|   |   P  Q  S   |   P  Q  S   |   P  Q  S   |   P  Q  S   |
| A | **1** 1  2  |   **1** 1 1 |   2  2  3   |**0** 1  1   |
| B |   2  0  0   |   1  1  0   |   3  1  0   |             |
| C |   1  2  1   |   3  2  1   |   4  4  2   |             |
| D |   0  1  1   |   4  4  4   |   4  5  5   |             |

3. Does a safe sequence exist?
    1. Can A be fullfilled?
        1. NO
    2. Can B be fullfilled?
        1. NO
    3. Can C be fullfilled?
        1. NO
    4. Can D be fullfilled?
        1. NO
    5. NO
4. Don't grant the request

##### 1.6.1.2.2. Example 2

Process A requests `1` unit of resource `S`.

1. Is the request smaller than need?
    1. YES
2. Is the request smaller than available?
    1. YES
    2. update Allocated and Need of A and Available

|   | Allocated   | Need        | Max         | Available   |
|---|-------------|-------------|-------------|-------------|
|   |   P  Q  S   |   P  Q  S   |   P  Q  S   |   P  Q  S   |
| A |   0  1 **3**|   2  1 **0**|   2  2  3   |   1 1 **0** |
| B |   2  0  0   |   1  1  0   |   3  1  0   |             |
| C |   1  2  1   |   3  2  1   |   4  4  2   |             |
| D |   0  1  1   |   4  4  4   |   4  5  5   |             |

3. Does a safe sequence exist?
    1. Can A be fullfilled?
        1. NO
    2. Can B be fullfilled?
        1. YES
        2. update Available and safe sequence
    3. Can C be fullfilled?
        1. NO
    4. Can D be fullfilled?
        1. NO
    5. Can A be fullfilled?
        1. YES
        2. update Available and safe sequence
    6. Can C be fullfilled?
        1. YES
        2. update Available and safe sequence
    7. Can D be fullfilled?
        1. YES
        2. update Available and safe sequence
    8. YES, safe sequence is `<B, A, C, D>`
4. Grant the request

#### 1.6.1.3. Detecting Deadlocks

Use the Bankers Algorithm to detect if any processes cannot be finished, if yes there must be a deadlock since Max must be smaller or equal than the total number of resources.

#### 1.6.1.4. Deadlock Handling

- Ignore Deadlocks
    - used by UNIX
    - Reboot necessary
- Prevent Deadlocks
    - Keep one of the four requirements from happening(see [Lecture Notes](https://github.com/JFKakaJFK/informatik-bsc/blob/master/semester%202/voos/Lecture%20Notes/Teil%206%20Deadlocks.pdf) (Page 17))
- Deadlock Avoidance
    - Do not grant unsafe Requests(Bankers Algorithm w/ Requests)
    - Keep the system in a safe state(always a safe path)
    - If the system is unsafe deadlocks may happen but not necessarily
- Deadlock Recovery
    - Kill all involved processes
    - Reset all involved processes(deadlock may reappear)
    - As long as the deadlock does not disappear, kill one of the involved processes
    - Take resources from the involved processes

### 1.6.2. Old Exam Questions

#### 1.6.2.1. Example 1

A distributes System uses mailboxes to communicate. There are two operations, send(sends a mail to a receiver) and receive(waits until a mail from a specified receiver is received). Are deadlocks possible? Give a detailed answer.

Yes deadlocks are possible since the prerequesites for deadlocks(hodl and wait, mutual exclusion and no voluntary backoff) are possible and circular wait is possible. Consider the following code:

| Program A | Program B |
| --- | --- |
| `...` | `...` |
| `receive(B)` | `receive(A)` |
| `send(B)` | `send(A)` |

Both A and B block until they receive a message from each other(hodl and wait), they will only accept a message from a specific process(mutual exclusion), no one will stop waiting and send(no vouluntary backoff) and circular wait(A waits for B, B waits for A) will happen, therefore deadlocks are possible.

#### 1.6.2.2. Example 2

A PC has 6 disks and `n` processes which all need up to two of them. For which `n` are deadlocks impossible?

In order for a deadlock to exist, the four criteria have to be met. Considering this example, none of the processes may finish and free its two disks. Since each process needs **up to** two disks to finish, an order of operations should exist where no process should be able to get two disks. Lets start with `n = 6`:

If each process acquires one disk, no other process can finish since the second needed disk cannot be acquired.

Alright deadlocks can exist, lets try a lower `n`. If `n = 5`, and each process acquires one disk, one disk is still left and at least one process can finish, resulting in two free disks and so on.

Since at least one process is able to get its two disks, no deadlocks can happen(pigeonhole principle).

Since for all `n < 5` the same argument works, all `n` with `n < 6` are guaranteed deadlock free.

Looking at this problem from a mathematical standpoint, a function from the disks `D` to the processes `P` is sought, where at least two disks are assigned to the same process. This requirement can only be met if there is at least one disk more than there are processes. For `#(D) = 6`, `#(P)` must be less than 6.

## 1.7. Virtual Memory & Memory Allocation

### 1.7.1. Types of realization of virtual Memory

#### 1.7.1.1. Base & Bound and Segmentation

#### 1.7.1.2. Paging

##### 1.7.1.2.1. Working Set Strategy

##### 1.7.1.2.2. OPT (Optimal)

##### 1.7.1.2.3. FIFO

##### 1.7.1.2.4. LRU (least recently used)

##### 1.7.1.2.5. Clock Policy

### 1.7.2. Allocators and Garbage Collectors

#### 1.7.2.1. Best Fit Allocator

#### 1.7.2.2. First Fit Allocator (LIFO vs FIFO)

#### 1.7.2.3. Slab Allocator

#### 1.7.2.4. Free List Allocator

#### 1.7.2.5. Stop and Copy Garbage Collection

#### 1.7.2.6. Concurrent Garbage Collection

#### 1.7.2.7. Reference Counting Garbage Collection

## 1.8. I/O

Basically everything thats not a CPU, Data-Bus or Storage.

**Fun trivia fact:** bus, the communication system originates from 'omnibus' e.g. is literally understood as a bus on which information travels from A to B.

### 1.8.1. Memory Mapping

I/O devices are mapped into the memory space, making I/O devices accessible like files (UNIX).

Advantages:

- Control registers are just pointers in memory
- No special protection needed to keep user processes from performing I/O (since memory is already protected)
- I/O handled like memory -> e.g fopen works also for I/O (no extra I/O functions needed)

Disadvantages:

- A device control register cannot be cached (don't know if ready when cached)
- Adds a lot of overhead for **single bus** systems(I/O takes away bandwidth)

Single vs Dual bus

Single bus: A single bus between CPU, Memory and I/O

Dual bus: A bus between CPU and Memory and one between CPU, Memory and I/O

The Cpu needs to adress the device controller to exchange data, this wastes CPU time for I/O transfer.

### 1.8.2. DMA (Direct Memory Access)

A DMA Controller is a 'kind of CPU' wich controls I/O operations and therefore freeing the CPU from wasting time on accessing device controllers(CPU makes request to DMA conroller which interrupts the CPU when the I/O operation is done).

### 1.8.3. HDDs

More or less just multiple CDs stacked with multiple read heads -> more bandwidth. AND MAGNETIC which makes it cooler.

The access to the stored data should be as fast as possible. HDDs a lot slower than the CPU(ms vs ns). If there are multiple I/O requests, the order of completion needs to be scheduled.

Speedup Options: RAID(Redundant Array of Inexpensive Disks) for speedup and redundancy

#### 1.8.3.1. FCFS

First I/O request gets answered first. Easy and fair, yet not necessarily the fastest.

##### 1.8.3.1.1. Example

Read head starts at `53`

Queued Requested Blocks: `98, 183, 37, 122, 14, 124, 65, 67`

Completion order: `53 -> 98 -> 183 -> 37 -> 122 -> 14 -> 124 -> 65 -> 67`

Total Blocks travelled: `98-53 + 183-98 + |37-183| + 122-37 + ... + 67-65 = 636`

Average Blocks travelled: `636 / 8 ~ 80` Blocks per request

#### 1.8.3.2. SPTF(Shortest Positioning Time First)

Choose the request next with shortest postioning time/seek time (= Shortest Seek Time First(SSTF)). This is better than FCFS since locality is taken advantage of and increases throughput. Since there may always arrive a closer request, some requests could starve.

**NOTE:** according to the lecture notes this is not circular (e.g `0 -> last block` is worst case)

##### 1.8.3.2.1. Example

Read head starts at `53`

Queued Requested Blocks: `98, 183, 37, 122, 14, 124, 65, 67`

Completion order: `53 -> 65 -> 67 -> 37 -> 14 -> 98 -> 122 -> 124 -> 183`

Total Blocks travelled: `65-53 + 67-65 + ... + 183-124 = 236`

Average Blocks travelled: `236 / 8 ~ 30` Blocks per request

#### 1.8.3.3. Aged SPTF(Shortest Positioning Time First)

**Note:** be careful, the initial direction is to the nearest request if not specified otherwise.

SPTF but to combat starvation, the longer a request waits the sooner it will be answered. The used algorithm is `Effective Time = Positioning Time - Factor * Waiting Time`.

#### 1.8.3.4. SCAN, CSCAN & VSCAN

SCAN(Elevator) scheduling basically scans all the blocks between the lowest and highest request in the disk ensuring no starvation whilst still using locality. This scheduling algorithm favours the middle cylinders and may sometimes be worse than SPTF(SSTF).

**Note:** Basically like a windshield wiper where the requests are the raindrops.

CircularSCAN(CSCAN) is just SCAN from the first to the last but circular, meaning `last block -> 0` is a short distance.

VSCAN Mix between SSTF(SPTF) and SCAN, combining boths strenghs and weaknesses. Depending on a correction factor either SPTF(`r = 0`) or SCAN(`r = 1`). Basically SCAN requests can starve if there are infinitely many requests between the head position and the end in the current direction but requests behind the head are waiting. VSCAN makes this impossible by adding time stamps and forcing older requests not to starve.

##### 1.8.3.4.1. Example SCAN

Read head starts at `53`

Queued Requested Blocks: `98, 183, 37, 122, 14, 124, 65, 67`

Completion order: `53 -> 65 -> 67 -> 98 -> 122 -> 124 -> 183 -> 37 -> 14`

Total Blocks travelled: `299`

Average Blocks travelled: `299 / 8 ~ 37` Blocks per request

##### 1.8.3.4.2. Example CSCAN

Read head starts at `53`

Queued Requested Blocks: `98, 183, 37, 122, 14, 124, 65, 67`

Completion order: `53 -> 65 -> 67 -> 98 -> 122 -> 124 -> 183 -> 199 -> 0 -> 14 -> 37`

Total Blocks travelled: `184`

Average Blocks travelled: `184 / 8 = 23` Blocks per request

## 1.9. Multiprocessor & Multicomputer

### 1.9.1. Multiprocessor

#### 1.9.1.1. Uniform Memory Access(UMA)

#### 1.9.1.2. NUMA (Non-UMA)

#### 1.9.1.3. Chaches

##### 1.9.1.3.1. Cache coherence

##### 1.9.1.3.2. Write Back

##### 1.9.1.3.3. Write Through

#### 1.9.1.4. Scheduling of Multiprocessor Systems

### 1.9.2. Multicomputer

#### 1.9.2.1. Blocking vs. Nonblocking

#### 1.9.2.2. Distributed Shared Memory(DSM)

## 1.10. Virtualization

### 1.10.1. Hypervisors/VMM (Virtual Machine Monitor)

### 1.10.2. Automation

### 1.10.3. Dynamic Resource Management

### 1.10.4. Elastic I/O

## 1.11. References

- Lecture Notes (seriously nope)
- [GeeksforGeeks](https://www.geeksforgeeks.org/operating-systems/)
- [Old (2000) Exam with solution](https://github.com/JFKakaJFK/informatik-bsc/blob/master/semester%202/voos/1%20Ehemalige%20Klausuren/test_iv-15052000.pdf)