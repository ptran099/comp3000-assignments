>>> Week 1: Admins & Intro
    >> Terms:
        Multitasking: 
        Multiprogramming:
            -> For maximizing CPU utilization
        thread pools:
        fork-join
        grand central dispatch
        On boot computer starts bootstrap program
        Burst:
        CPU register
        Device controller
        Memory content
        DMA 
        NUMA Non-uniform memory access
        SMP (Symmetric Multiprocessing)
        PCB (Process Control Block): Holds process data like PID, state, memory info, CPU scheduling info, etc.
        Priority:
        Period:
        Program: executable, instructions stored on disk
        Process: program is active, in memory
        context switch: when going from user mode to kernel mode and vice versa

    >> System calls:
    Types of syscalls:
        process control
        file management
        device management
        information maintenance
        communication
        protection
        fork()
        exit() : process asks OS to delete it (process termination)
        wait() : process may return status value using this
                all resources are deallocated by the OS
        pipe()


>>> Week 2: OS Structures, Processes, IPC
    Process states:
        1. new
        2. ready
        3. running
        4. blocked (waiting)
        5. terminated

    Difference between waiting and ready
    Difference between non-blocking wait


    Process structure: 
        - text section
        - data section
        - program counter
        - stack: temporary data including function params, return address, local vars
        - heap: dynamically allocated memory during runtime

    Producer-Consumer problem
        - one solution is shared memory
        - to allow producer and consumer to run concurrently we use item buffer
        bounded buffer: fixed buffer size
            > consumer must wait if buffer is empty
            > producer must wait if buffer is full
        unbounded buffer
            >consumer may have to wait for new items
            >producer can always produce new items
            counter++ everytime produces add new item
            counter-- everytime consumer remove item
            context switch can create race condition
>>> Week 3: Threads and Scheduling
    Contrast threads vs processes
    Multithreaded process: all threads have their own register, stack, and program counter
    all the threads share the same code, data, and files segment
    
    Threads
        -> data
        -> code segment
        -> heap
        -> context
        -> UID

    User threads
    Kernel threads
    Thread Control Block is managed by

    Soft realtime: Critical realtime tasks have highest priority but no guarantee
    Hard realtime: task must be serviced by deadline

    CPU scheduling decisions may take place when:
    1. Switch from running to waiting
    2. Switch from running to ready
    3. Switch from waiting to ready
    4. Terminates
    Case 1 and 4: no choice in ready queue management; nothing is added into queue, only the head popped
    Case 2 and 3: have choice; do i stick with the one that ran before the event?

>>> Week 4: Scheduling and Synchronization
    CPU Scheduling
    Preemptive: When process can be stopped by some other process, and some other process can get
    the CPU while the previous process was still executing.
    Non-preemptive: When CPU can't be taken away from process until it completes it execution or waits for IO interrupt

    preemptive
    non-preemptive
    Batch scheduling
    interactive scheduling
    Realtime scheduling
        EDF
        rate monotonic
        proportional share 


    response time

    >> Scheduling policies/algorithms:
        1. FCFS (First come first serve) / FIFO (First in first out)
            Turnaround time = Completion time - Arrival time
            Benefits: Simple to implement
            Problems: convoy effect
            longer waiting time
            Non-preemptive
            if not given arrival time assume they all arrive at same time
        2. SJF (Shortest job first)
            Non-preemptive
            shortest next cpu burst
            Preemptive version is SRTF
        3. SRTF (Shortest remaining time first)
            Preemptive version of SJF
        4. RR (Round robin)
            Waiting time = Turnaround time - Burst time
            best non-multilevel algorithm for timeshare
        5. Priority Scheduling
            -> Priority is determined by periods, shorter periods higher prio
                -> have already seen the convoy effect from fcfs
            -> Priority scheduling with round robin
            -> Lower priority number goes first
        6. Multilevel queue
            Partitions the ready queue into several separate queues
            Processes are permanently assigned to one queue
            Each queue has its own scheduling algorithm
            There is scheduling among the queues as well
            Batch (background)
            Interactive (foreground)
            ->Multilevel feedback queue w/ priority
                allow process to move between queues
                separate processes according to their CPU burst characteristics
                if process uses too much CPU time, moved to lower priority queue

        EDF (Earliest deadline first)
            -> higher priority according to earlier deadline

    Critical section
    -> works with shared variables
    entry section
    critical section
    exit section
    remainder section

    Critical Section Problem
        Solution specs for CSP
        1. Mutual Exclusion
        2. Progress: if no process is executing in its critical section and there exists some process that wishes to enter their critical section, then the selection of the process that will enter the critical section next cannot be postponed indefinitely
        3. Bounded wait: bound must exist on number of times other processes are allowed to enter CS after a process has made a request to enter CS and before that request is granted. Fixed limit on number of times same process can execute the critical section to prevent starvation/deadlocks. (Portapotty)
        
        does bounded wait hold?
        
        Peterson's solution (Software solution)
            int turn: indicates whose turn it is to enter the critical section
            boolean flag[2]: indicates if a process is ready to enter critical section
            When process P_i wants to enter CS, it sets flag[i] to true
        Test and Set (Hardware solution)
            shared lock variable
            1 means locked, so keep waiting
            if not locked, take the lock and executes critical section
            atomic operations


    Mutex
    Semaphore use 
    counting semaphore
    wait() when it needs to use resource
        decrement
    signal() to release resource
        increment
    binary semaphore
        0: resource is being held
        1: resource is free

>>> Week 5: Synchronization & Deadlocks
    Bounded buffer problem
    Readers-writers problem
    Dining philosopher's problem
        -> shared resources
        -> we solve this using semaphores

    4 conditions for deadlock:
        1. Mutual exclusion: only one thread has access to resource at given time
        2. Hold and wait:
        3. No preemption:
        4. Circular wait:

>>> Week 6: Deadlocks & memory management
    On single processor systems CPU goes idle during IO operations which is wasted time

    With multiprogramming OS takes CPU from process that's waiting and gives CPU to another process

    How long should process be given

    Assign particular CPU time to certain process at certain time

    When CPU has begun execution it is either 2 states: execution, or IO wait

    Test and swap
    Deadlock avoidance
    -> Banker's algorithm



>>> Problems
    Definitions


    Multiple choice
    Gantt chart for scheduling
    -> Round robin:
        -> Make a ready queue
        -> Draw a timeline
        -> Make a clock to track arrival time and job burst WRT quanta
    -> FCFS
        ->
    Process state chart
        > process, start state at transition, end state at transition
