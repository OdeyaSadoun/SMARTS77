/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory                 */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum                  */
/* Jerusalem College of Technology, 5759-64 (1999)                            */
/* SchedAlgo64.CPP, short-term scheduler algorithms                */
/****************************************************************/


#include "smarts77.h"

int roundRobin( )
// Round Robin short-term algorithm 
{
	int count;
	int nextTask = (SMARTS.getCurrentTask()+1)%SMARTS.getTotalTasks( );
	for (count=0;
	     SMARTS.getStatus(nextTask)!=READY && count<SMARTS.getTotalTasks( );
	     count++)
	     nextTask=++nextTask%SMARTS.getTotalTasks( );
	if (count == SMARTS.getTotalTasks( ))	// no found READY task to run
		nextTask = SMARTS.getTotalTasks( );
	return nextTask;
}

int EDF( )
{
	int nextTask = SMARTS.getTotalTasks(); //enter the last task to the nextTask
	int min = MAXINT; //enter to min the max value in order to find the task with minimum cycle time
	for (int i = 0; i < SMARTS.getTotalTasks(); i++)
	{
		if (SMARTS.getStatus(i) == READY && SMARTS.getHowMuchTimeLeft(i) < min)
		{
			min = SMARTS.getHowMuchTimeLeft(i);
			nextTask = i;
		}
	}
	return nextTask;
}

int RMS( )
{
	int nextTask = SMARTS.getTotalTasks(); //enter the last task to the nextTask
	int min = MAXINT; //enter to min the max value in order to find the task with minimum cycle time
	for (int i = 0; i < SMARTS.getTotalTasks(); i++)
	{
		if (SMARTS.getStatus(i) == READY && SMARTS.getTaskCurrentPriority(i) < min)
		{
			min = SMARTS.getTaskCurrentPriority(i);
			nextTask = i;
		}
	}
	return nextTask;
}
