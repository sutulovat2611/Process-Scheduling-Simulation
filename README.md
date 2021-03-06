# Process-Scheduling-Simulation
Creating three different scheduling programs that each implement a different process  scheduling algorithm (FCFS, SRTN with q=3, Deadline-based) to simulate the scheduling of processes in a system. 

# fcfs.c:
Program fcfs.c is a simulation of non-preemptive First Come First 
Serve (FCFS) scheduling algorithm. It gets the information about each process 
from the txt file, where each line represents a singe process in form of:
PROCESS NAME | ARRIVAL TIME | SERVICE TIME | DEADLINE
------------ | ------------ | ------------ | ------------
Any name of the proccess, e.g. P1 | Time when the process enters the system | Time when the process should run till the end of execution | The maximum time the process should take from its entry to the system till the end of execution

- The file may be specified by the user or, if not, the default "processes.txt"
is to be used.
- It schedules the processes in order of arrival, printing out the main events:
 	- when the new process enters
 	- when any of the processes finishes execution
 	- which process is currently running ( when any of the events above happen)

- Moreover, it saves information about each of the processes into results-1.txt
in form of:

PROCESS NAME | WAIT TIME  | TURNAROUND TIME | DEADLINE MET 
------------ | ------------ | ------------ | ------------
Any name of the proccess, e.g. P1 | Time from arrival till the process started running | Time from the start till the end of the process execution | 1 if met, 0 otherwise	
    
## In order to run the program:
1. Compile it with: ```gcc task1-30806151.c ```
2. Run it without a specified file (it will use the default "processes.txt"):
  ```./a.out ```
3. Run it with the specified file "filename.txt": ```./a.out filename.txt```
	 	
## Assumptions:
1. The process is deleted from the queue, when all it's attributes are set to -1/"-1" and it's state is EXIT.
2. Process name cannot be "-1". It will break the program
3. Front pointer is always pointing at the process that is in the RUNNING state. 
4. When two processes have the same entry time, the one which is earlier in the file, is to be executed first.
5. Maximum length of the file line may be 128 char
6. All the numbers must be positive, whole numbers
	
# srtnQ3.c:
Program srtnQ3.c is a simulation of the preemptive Shortest Remaining Time Next (quantum=3) 
(SRTN(q=3)) scheduling algorithm. It gets the information about each process from the file, where each
line represents a singe process in form of:
PROCESS NAME | ARRIVAL TIME | SERVICE TIME | DEADLINE
------------ | ------------ | ------------ | ------------
Any name of the proccess, e.g. P1 | Time when the process enters the system | Time when the process should run till the end of execution | The maximum time the process should take from its entry to the system till the end of execution

It schedules the processes, choosing the shortest remaining time 
process next and switching to the next one after 3 seconds __OR__ if the current 
one is completed and then prints out the main events:
- when the new process enters
- when any of the processes finishes execution
- which process is currently running ( when any of the events above happen)
 	
Moreover, it saves information about each of the processes into results-2.txt in form of:
PROCESS NAME | WAIT TIME  | TURNAROUND TIME | DEADLINE MET 
------------ | ------------ | ------------ | ------------
Any name of the proccess, e.g. P1 | Time from arrival till the process started running | Time from the start till the end of the process execution | 1 if met, 0 otherwise	
   
 		
## In order to run the program:
1) Compile it with: ```gcc task2-30806151.c```
2) Run it without a specified file (it will use the default "processes.txt"): ```./a.out ```
3) Run it with the specified file "filename.txt": ```./a.out filename.txt```
	
## Assumptions:
1. The process is deleted from the queue, when all it's attributes are set to -1/"-1" and it's state is EXIT.
2. Process name cannot be "-1". It will break the program
3. Maximum length of the file line may be 128 char
4. All the numbers must be positive, whole numbners
5. If two processes have the same remaining time, the one which arrived to the
queue first will be processed first.
	
# deadline_based.c:
Program deadline_based.c is a simulation of the preemptive deadline-driven(q=1) scheduling 
algorithm. It gets the information about each process from the file, where each
line represents a singe process in form of:

PROCESS NAME | ARRIVAL TIME | SERVICE TIME | DEADLINE
------------ | ------------ | ------------ | ------------
Any name of the proccess, e.g. P1 | Time when the process enters the system | Time when the process should run till the end of execution | The maximum time the process should take from its entry to the system till the end of execution			
   
It schedules the processes, choosing the process, which is still able to end processing 
within the deadline and which is going to reach the deadline 
the fastest; if there is no process like that, works as FCFS) and then prints out the main events:
  - when the new process enters
  - when any of the processes finishes execution
  - which process is currently running ( when any of the events above happen)

Moreover, it saves information about each of the processes into results-3.txt in form of:
Moreover, it saves information about each of the processes into results-2.txt in form of:
PROCESS NAME | WAIT TIME  | TURNAROUND TIME | DEADLINE MET 
------------ | ------------ | ------------ | ------------
Any name of the proccess, e.g. P1 | Time from arrival till the process started running | Time from the start till the end of the process execution | 1 if met, 0 otherwise	
  
  
 		
## In order to run the program:
1) Compile it with: ```gcc task3-30806151.c ```
2) Run it without a specified file (it will use the default "processes.txt"): ```./a.out ```
3) Run it with the specified file "filename.txt":  ``` ./a.out filename.txt```
	
## Assumptions:
1. The process is deleted from the queue, when all it's attributes are set to -1/"-1" and it's state is EXIT.
2. Process name cannot be "-1". It will break the program
3. Maximum length of the file line may be 128 char
4. All the numbers must be positive, whole numbners
5. If two processes have the same remaining time till the deadline is be passed,
the one with the shorter deadline will be picked. If both deadlines are the same, 
the process which arrived to the queue earlier will be processed next.
	
## How and why an algorithm works:
1. The process switch happens every second, making it dynamically switch 
between processes immideately if, for example, the current process has already passed the deadline. 
This helps to prevent wasting time on the processes, which are already over the deadline or 
which are impossible to complete within the deadline. 
2. Deadline represents the expected turnaround time, and therefore,
the next process to be executed is chosen based on how much time
is left from the current moment till the deadline is passed. (This is to 
be referred as a gap). __Gap is calculated by:__
```gap = deadline of the process - (current time - its entry time).```      
The smaller the gap is, more likely the process will pass it deadline sooner.
4. If two processes have the same gap, the one with the smaller deadline,
is more likely to be completed sooner and therefore it is chosen to run next.
5. If the gap is negative, the process has already passed it's deadline,
meaning there is no point to waste time on it, and therefore this process
will be executed after all the processes which have not yet passed the deadline
6. Bedsides negative gap, it also prioritizes the processes, whose gap is
smaller than or equals to their remaining time. Since otherwise, it means
that the process will not be finished before the deadline, stealing time
from the other processes.
7. All the processes that passed their deadline are pushed to the end of execution 
and they are executed in FCFS algorithm. 
## Example:
PROCESS NAME | ARRIVAL TIME | SERVICE TIME | DEADLINE
------------ | ------------ | ------------ | ------------
P1 | 0 | 3 | 10
P2 | 1 | 5 | 6
	
		
		P1 enters first and is executed first. It's gap is
			gap = 10 - (1 - 0) = 9;
		P1 runs;
		At second 1, P2 enters the system. It's gap is
			gap = 6 - (1 - 1) = 6;
		P2 has a lower gap, which will make the algorithm choose it next.
		
		P2 runs, gap 5 | P1 waits, gap 8
		P2 runs, gap 4 | P1 waits, gap 7
		P2 runs, gap 3 | P1 waits, gap 6
		P2 runs, gap 2 | P1 waits, gap 5
		P2 runs, gap 1 | P1 waits, gap 4
		P2 finished execution, meeting the deadline| P1 runs, gap 3
		P1 runs, gap 3
		P1 runs, gap 2
		P1 finishes execution, meeting the dealine
		
		Both processes met the deadline.
