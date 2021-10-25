/*
 * Name: Sutulova Tatiana
 * Start date: 01/10/2021
 * Last modified: 06/08/2021
 * -------------------------
 * 
 * The following application is a simulation of non-preemptive FCFS scheduling 
 * algorithm. It gets the information about each process from the file, where each
 * line represents a singe process in form of:
 * 		[Process Name] [Arrival Time] [Service Time] [Deadline]. 
 *  It schedules the processes accordingly ( in order of arrival), printing out the main events:
 *		- when the new process enters
 *		- when any of the processes finishes execution
 *		- which process is currently running ( when any of the events above happen)
 *
 * Moreover, it saves information about each of the processes into results-1.txt
 * in form of:
 * 		[Process name] [Wait time] [Turnaround Time] [Deadline Met]
 */

#include <stdio.h>      /* standard I/O routines */
#include <stdlib.h>     /* needed for exit function */
#include <string.h>     /* string operations */
#define MAX 100



/*Special enumerated data type for process state*/
typedef enum {
	READY, RUNNING, EXIT
} process_state_t;

/* C data structure used as process control block. The scheduler should create
one instance per running process in the system*/
typedef struct{
	char processname[11]; // A string that identifies the process
	
	/*Times are measured in seconds*/
	int entryTime; // The time process enteres system
	int serviceTime; //The total CPU time required by the process
	int remainingTime; //Remaining service time until completion 
	
	int deadline; //The expected turnaround time
	
	process_state_t state; //current process state(e.g. READY)
} pcb_t;

/*global variables*/
int rear= 0; //points at the last inserted process
int front=0; //points at the process being currently processed
pcb_t queue[MAX];

/*functions*/
void add_empty(int count);
void insert();
void delete();
void fill_empy_process();
int new_process(FILE *fp);
void remaining_processes(int time);
void add_empty_processes(FILE *fp);
void summarize (pcb_t process, int time);
void clear_output_file(); 



/*
 * Function: main 
 * --------------
 * opens the file for reading from it, checking whether the user input is valid. 
 * if the user did not specify the file to read from, opens the default "processes.txt",
 * otherwise opens the specified file. 
 *
 * calls the clear_output_file() function to clear the "results-1.txt" file
 *
 * calls the add_empty_processes() function to initialize all the processes with default numbers
 *
 * calls the scheduling functions to schedule the processes with FCFS approach
 *
 * argc: argument counter, which is the number of arguments the user input
 * argv: array containing all the arguments the user input 
 * 
 * returns: none
 */
int main(int argc, char* argv[])
{
	/*open files*/
	FILE *fp;
	if (argc>1){
		fp = fopen (argv[1], "r");
	}
	else{
		fp = fopen ("processes.txt", "r");
	}
	
	/*handling the non-existent file*/
	if (fp==NULL){
		printf("The file could not be open. Try again");
		exit(0); 
	}
	/*clear the output file*/
	clear_output_file();
	
	/*initialize time*/
	int time=0;
	
	/*counting number of lines in the file and adding and empty processes to the queue
	for each line*/
	add_empty_processes(fp);
	
	/*adding processes and working on them*/
	time=new_process(fp);
	
	time-=1; //needed since after the last process is added, time is increased 
	
	/*after all the processes are added, work on the processes which remain in the system*/
	remaining_processes(time);
	
	/*closing the open file*/
	fclose(fp);
	
	exit(0); 
}


/*
 * Function: clear_output_file
 * -------------------
 * clears the content of the "results-1.txt" file
 *
 * returns: none
 */
void clear_output_file(){
	fclose(fopen("results-1.txt", "w"));
}


/*
 * Function: insert
 * -------------------
 * inserts the process into the queue, checking whether total number does not excess the
 * limitation. Places the rear pointer at the last inserted process.
 * 
 * process: the instance of pcb_t struct, which is to be inserted
 *
 * returns: none
 */
void insert(pcb_t process){
	if (rear == MAX ){
		printf("The number of processes is exceeding the limit");
		exit(0); 
	}
	else{
		//if the queue has no processes
		if (front == -1){
			front = 0;
		}
		queue[rear]= process;
		rear=rear+1;
	}
}

/*
 * Function: delete
 * -------------------
 * deletes the front process from the queue by calling add_empty() function
 * and resetting front pointer to the next process
 *
 * returns: none
 */
void delete(){
	add_empty(front);
	front+=1;
}


/*
 * Function: add_empty
 * -------------------
 * adds the process with all the default attributes to the given position
 *
 * count: position of the process to be deleted
 *
 * returns: none
 */
void add_empty(int count){
	pcb_t process; 
	strcpy(process.processname, "-1");
	process.entryTime= -1;
	process.serviceTime =-1;
	process.remainingTime=-1;
	queue[count]= process;
}

/*
 * Function: new_process
 * -------------------
 * works on scheduling the processes, when there are processes that are NOT read from the
 * file. reads the file line by line, inserting the processes to the queue at their 
 * entry time, while allowing other process to run
 *
 * fp: file to be read from
 *
 * returns: none
 */
int new_process(FILE *fp){
	/* initialization*/
	pcb_t process;
	size_t len = 128; //maximum length of line is 128 char
	char *line= malloc(sizeof(char)*len); //allocating space
	int time=0; //current time in seconds
	int inserted; // shows whether the character was inserted or not
	
	while (fgets(line, len, fp)!=NULL){ //while there is a line in the file to be read
		/*reading new line into process*/
		sscanf (line, "%10s%d%d%d", process.processname, &process.entryTime, &process.serviceTime, &process.deadline);
		process.remainingTime=process.serviceTime;
		
		inserted=0; //reinitializing to 0 for every new line
		
		while (inserted == 0){
			/*insert the process once the time reaches it's enrty time*/
			if (process.entryTime <= time){
				insert(process); 
				queue[rear].state=READY; // process is in a ready state once inserted
				printf("Time %d: %10s has entered the system\n", process.entryTime, process.processname );
				inserted = 1; //shows that it was inserted
				if (!strcmp(process.processname,queue[front].processname ))
					printf("Time %d: %10s is in the running state\n", time, queue[front].processname );
			}
			
			/*ensuring the process exists*/
			if (strcmp (queue[front].processname , "-1" )){
				queue[front].state=RUNNING;
				/*deduct one second, if remaining time is more than 0*/
				if (queue[front].remainingTime!=0){
					queue[front].remainingTime-=1;
				}
				/*once remaining time is 0, process has finished the execution*/
				else if (queue[front].remainingTime==0){
					queue[front].state=EXIT;
					printf("Time %d: %10s has finished execution\n", time, queue[front].processname);
					summarize(queue[front], time);
					delete(); //process is removed from the queue
					printf("Time %d: %10s is in the running state\n", time, queue[front].processname );
				}	
			}
			time+=1; //incrementing every second
		}
	}
	free(line); //empty allocated space
	return time;
}


/*
 * Function: remaining_processes
 * ------------------------------
 * works on scheduling the processes, when ALL the processes are read from the
 * file. works on the remaining processes, scheduling them according to FCFS algorithm
 *
 * time: time which the new_process() stopped running at
 *
 * returns: none
 */
void remaining_processes(int time){
	
	/*works on the remaining processes*/
	while(front<rear){ 
		/*once remaining time is 0, process has finished the execution*/
		if (queue[front].remainingTime==0){
			queue[front].state=EXIT; //resetting it's state to exit
			printf("Time %d: %10s has finished execution\n", time, queue[front].processname);
			summarize(queue[front], time);
			delete(); //process is removed from the queue
			if (front<rear)
				printf("Time %d: %10s is in the running state\n", time, queue[front].processname );
		}
		queue[front].state=RUNNING;
		queue[front].remainingTime-=1; //decrement remaining time every iteration
		time+=1; //incrementing every second
	}
}

/*
 * Function: add_empty_processes
 * -----------------------------
 * loops through each line of the file ( which corresponds to a single process) 
 * and inserts the default empty processes there ( by calling add_empty()).
 *
 * fp: file to be read from
 *
 * returns: none
 */
void add_empty_processes(FILE *fp){
	char c; 
	int count=0;
	for (c= getc(fp); c!= EOF; c= getc(fp))
		if (c =='\n')
			add_empty(count); //adds an empty process to the queue
			count+=1; //increased when a single line of a while is read
	rewind(fp); //reset the pointer back to the beginning of the file
}

/*
 * Function: summarize
 * -------------------
 * Calculates process's wait time, turnaround time and determines whether the 
 * deadline was met. 
 * Writes all that information about the process to the file "results-1.txt"
 *
 *
 * process: process, that summary is about
 * time: time, when the process completed execution
 *
 * returns: none
 */
void summarize (pcb_t process, int time){
	/*Initialization*/
	int deadline_satisfied=0;
	int wait_time;
	int turnaround_time;
	/*Open the file*/
	FILE *fpw;
	fpw = fopen ("results-1.txt", "a");
	
	turnaround_time=time-process.entryTime; //calculating the turnaround time
	wait_time=turnaround_time-process.serviceTime; //calculating the wait time
	/*Determining whether the deadline is satisfied*/
	if (turnaround_time<=process.deadline){
		deadline_satisfied = 1;
	}
	// Adding the summary to the file
	fprintf(fpw, "%s %d %d %d\n", process.processname, wait_time, turnaround_time, deadline_satisfied);
	fclose(fpw);
}
