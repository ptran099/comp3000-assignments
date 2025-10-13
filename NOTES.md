On boot computer starts bootstrap program
CPU register
Device controller
Memory content

Terms:
DMA 
NUMA Non-uniform memory access
SMP (Symmetric Multiprocessing)

CPU Scheduling


Scheduling circumstances 1 to 4


On single processor systems CPU goes idle during IO operations which is wasted time

With multiprogramming OS takes CPU from process that's waiting and gives CPU to another process


How long should process be given

Assign particular CPU time to certain process at certain time

When CPU has begun execution it is either 2 states: execution, or IO wait

Scheduling algorithms

Shortest Job first

Round-robin
quanta


Test and swap

Peterson's Solution
