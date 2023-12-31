/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory            */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum              */
/* Jerusalem College of Technology, 5759-64 (1999)                  */
/* update  Tishrey   5777                                           */
/* SMARTS77.H, main header file                                     */
/********************************************************************/

#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <values.h>
#include <iostream.h>

/**********    Constants    **********/
#define true 1
#define false 0
#define timerInt 0x08
#define userInt 0x60
#define MaxTask 15
#define MaxStack 512
enum taskStatus
{
	READY,
	NOT_ACTIVE,
	SUSPENDED,
	SLEEP,
	UNDEFINED
};

/**********    Classes    **********/
class Event
{
private:
	int flag;		  // arrived flag
	void *data;		  // the parameter
	char source;	  // source task name
	char dest;		  // destination task name
	int senderWait;	  // tells if the sender has to wait till the reciver receives
	int testAndSet(); // test and set 'flag' in an 'atomic transaction'
public:
	Event();
	// sends an event to 'dest', if 'synch' is true waits until the receiver gets it.
	void send(char dest, void *param, int synch);
	// waits for the event, and returns the parameter.
	void *wait(char &sourceP); //'sourceP' will contain the task sender ID.
	// check for for an arrived event. 'sourceP' will contain the task sender ID.
	int arrived(char &sourceP);
	// resets the event
	void reset();
};

// SMARTS manages an entry of this type for each task
class Task
{
public:
	char name;					  // task name
	unsigned stack[MaxStack];	  // the stack
	unsigned copyStack[MaxStack]; // the copy of stack
	unsigned stackSeg, stackPtr;  // the stack pointer
	int contextSwitchFlag;
	taskStatus status;			  // the status of the task
	Event *expectedEvent;		  // the event that this task is waiting for
	int sleepCount;				  // the sleep remainder time
	int numCycle;				  // the num of cycle for the task
	int cycleTime;				  // the time that the task need in every cycle
	int currentCycleTime;		  // the current cycle time
	int priority; 				  // the priority of the task
	int currentPriority;

	Task();
	void declare(void far *code, void far *taskEnd, char name, int numCycle, int cycleTime);
	void sleepDecr();
	void incrPriority();
	void setOriginalPriority();
	void reDeclare();
	int getPriority();
	void setPriority();
};
//                           Main class
class Parallelism
{
private:
	Task context[MaxTask];		  // all tasks entries
	Task contextSched;			  // scheduler entry
	unsigned schedCopy[MaxStack]; // a copy of the scheduler stack
	int totalTasks;				  // total declared tasks
	int currentTask;			  // time slice holding task
	int deadlock;				  // deadlock flag
	int error;
	int progInt;		   // 'program interrupt' flag
 // enables/disables context switch
	int endOfTimeSlice;	   // indicates that has to be a context switch as soon as possible
						   // a pointer to the new timer interrupt handler
	void interrupt (*timerInterruptHandler)(...);
	// a pointer to the original BIOS timer interrupt handler
	void interrupt (*userIntAddress)(...);
	void far *scheduler;   // a pointer to the scheduler
	void far *userTaskEnd; // a pointer to the function called at the end of each task
public:
	int sleepTasks;			   // indicates how many task is sleeping, and it can't be deadlock
	int activeTasks;		   // indicates how many task is activate
	int traceInd;			   // debugging index
	long TScount;			   // counts the clocks of the current task
	unsigned timerClocksBegin; // time start of the current task
	unsigned timerClocksEnd;   // time finish of the current task
	int far (*algorithm)();	   // short-term scheduler algorithm function
	Parallelism();
	// sets the addresses of the external functions
	void externalFunctions(void interrupt (*timerInterruptHandler)(...),
						   void far *scheduler, void far *userTaskEnd,
						   int far (*algorithm)());
	int declareTask(void far *code, char name, int numCycle, int cycleTime); // inserts a new task entry
	void runTheTasks();														 // start running the tasks
	void callScheduler();													 // return to the control to the scheduler
	void restoreSchedStack();												 // restore the 'scheduler' stack
	int getCurrentTask();													 // get the current running task entry number
	void setCurrentTask(int taskNum);										 // set the next running task number
	int getTotalTasks();													 // get total declared tasks
	int getDeadlock();														 // get deadlock flag
	void setDeadlock();														 // set deadlock flag
	int contextSwitchOn();													 // enable context switch
	void contextSwitchOff();												 // disable context switch
	int getContextSwitch();													 // get 'context switch' flag status
	void setProgInt();														 // set the 'program interrupt' flag
	void resetProgInt();													 // clear the 'program interrupt' flag
	int getProgInt();														 // get the 'program interrupt' flag status
	void setEndOfTimeSlice();												 // set the 'end of time slice' flag
	char getName(int taskNum);
	char getCurrentName();
	taskStatus getStatus(int taskNum);
	taskStatus getCurrentStatus();
	void resume(int taskNum);
	void resume(char taskName);
	void setCurrentNotActive();
	void suspended();
	void incrPriority(int taskNum);
	void setOriginalPriority(int taskNum);
	void setCurrentOriginalPriority();
	Event *getExpectedEvent(int taskNum);
	Event *getCurrentExpectedEvent();
	void setCurrentExpectedEvent(Event *expectedEvent);
	void sleep(int t); // sleep current task for 't' milliseconds
	void sleepDecr(int taskNum);
	void getCurrentStack(unsigned &StackSeg, unsigned &StackPtr);
	void setCurrentStack(unsigned StackSeg, unsigned StackPtr);
	void getSchedStack(unsigned &StackSeg, unsigned &StackPtr);
	void handleTimers();
	void taskEnd();
	int getHowMuchTimeLeft(int taskNum);
	void setError();
	int getError();
	int allFinish();
	int getCycleTime(int taskNum);
	int getTaskCurrentPriority(int taskNum);
};

class PriorityQueue
{
private: 
	int arr[MaxTask]; //An array with size of num of tasks
	int numOfPendingTasks; //waiting tasks in queue (== arr.length)
public:
	PriorityQueue();
	void push(int taskNum);
	int pop();
	int isEmpty();
	int isFull();
};

class Mutex
{
private:
	int semaphore;
	PriorityQueue queue;
	int owner;
	int currentTask;
	int level;
	int inheritance;

public:
	Mutex(int inheritance);
	void acquire();
	void release();
};



//=======================
/**********    Functions    **********/
// reads the number of the remaining clocks from the timer register
extern unsigned getTimerClocks();
// external functions for 'externalFunctions' function
void far interrupt timerInterruptHandler(...);
void scheduler();
void myTaskEnd();
// 'short term' scheduler algorithms
int roundRobin();
int EDF();
int RMS();//

//heduler object
extern Parallelism SMARTS;
