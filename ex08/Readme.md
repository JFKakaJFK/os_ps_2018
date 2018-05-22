# Exercise Sheet 8

In this exercise sheet you will reiterate process scheduling.

# Task 1

Create an arbitrary set of input data of process arrival times, service times, priorities and any other configuration information required for a scheduling problem (e.g. quantum length for round robin scheduling).

This information could look as follows (taken from Exercise Sheet 2):

| Process | Arrival Time | Service Time | Priority |
|---------|--------------|--------------|----------|
| A       | 0            | 3            | 1        |
| B       | 2            | 6            | 1        |
| C       | 4            | 4            | 2        |
| D       | 6            | 5            | 2        |
| E       | 8            | 2            | 3        |

Note that it is NOT allowed to simply use the sample data set provided above.

Create a chart describing how these processes are assigned to the CPU according to different scheduling algorithms, and compute the

* *turnaround time*
* *response time*
* *throughput*

for the scheduling algorithms

1. FCFS (First Come First Served)
2. SPN (Shortest Process Next)
3. SRT (Shortest Remaining Time Next)
4. Round Robin
5. Highest Priority First with preemption
6. Highest Priority First without preemption
7. Lottery Scheduling

Note the following additional requirements:

* The number of processes must be between 4 and 6
* The processes' arrival times must be between 0 and 6
* The processes' service times must be between 2 and 6
* Any time quanta must be lower than 5

Also note that choosing the same value for e.g. the service times for all processes is not allowed. There must be some variation.

# Task 2

Create a second, different set of process data and compute the scheduling outcome as required for Task 1.