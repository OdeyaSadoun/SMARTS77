/********************************************************************/
/* A Small Real Time System for the Real-Time laboratory            */
/* built by: A.Teitelbaum on an idea of H.G.Mendelbaum              */
/* Jerusalem College of Technology, 5759-64 (1999-03)               */
/* update  Tishrey   5777                                           */
/* APP77.CPP, an application to demonstrate SMARTS77  functioning   */
/********************************************************************/
#include "smarts77.h"
Mutex ioMutex(0),qMutex(0);
Event evAtoC,evAtoB;
FILE *newFile;
// user Functions
void a()
{
	ioMutex.acquire();
	cout << "\n *************   A Start    *********************";
	newFile = fopen("newFile.txt", "a");
	fprintf(newFile, "\n *************   A Start    *********************");
	fclose(newFile);
	ioMutex.release();
	for (int j = 0; j < 10; j++)
	{
		for (long i = 0; i < 200000; i++)
			;
		ioMutex.acquire();
		cout << "A";
		newFile = fopen("newFile.txt", "a");
		fprintf(newFile, "A");
		fclose(newFile);
		ioMutex.release();
	}
	ioMutex.acquire();
	cout << "\n *************   A Finish   *********************";
	newFile = fopen("newFile.txt", "a");
	fprintf(newFile, "\n *************   A Finish   *********************");
	fclose(newFile);
	ioMutex.release();
}

void b()
{
	ioMutex.acquire();
	cout << "\n *************   B Start    *********************";
	newFile = fopen("newFile.txt", "a");
	fprintf(newFile, "\n *************   B Start    *********************");
	fclose(newFile);
	ioMutex.release();
	for (int j = 0; j < 10; j++)
	{
		for (long i = 0; i < 200000; i++)
			;
		ioMutex.acquire();
		cout << "B";
		newFile = fopen("newFile.txt", "a");
		fprintf(newFile, "B");
		fclose(newFile);
		ioMutex.release();
	}
	ioMutex.acquire();
	cout << "\n *************   B Finish   *********************";
	newFile = fopen("newFile.txt", "a");
	fprintf(newFile, "\n *************   B Finish   *********************");
	fclose(newFile);
	ioMutex.release();
}

void c()
{
	int j;
	ioMutex.acquire();
	cout << "\n *************   C Start    *********************";
	newFile = fopen("newFile.txt", "a");
	fprintf(newFile, "\n *************   C Start    *********************");
	fclose(newFile);
	ioMutex.release();
	for( j = 0; j < 5; j++)
	{
		for (long i = 0; i < 200000; i++)
			;
		ioMutex.acquire();
		cout << "C";
		newFile = fopen("newFile.txt", "a");
		fprintf(newFile, "C");
		fclose(newFile);
		ioMutex.release();
	}

      char ch;
	  evAtoC.wait(ch);
	for( j = 0; j < 5; j++)
	{
		for (long i = 0; i < 200000; i++)
			;
		ioMutex.acquire();
		cout << "C";
		newFile = fopen("newFile.txt", "a");
		fprintf(newFile, "C");
		fclose(newFile);
		ioMutex.release();
	}

    qMutex.acquire();

		for( j = 0; j < 5; j++)
	{
		for (long i = 0; i < 200000; i++)
			;
		ioMutex.acquire();
		cout << "C";
		newFile = fopen("newFile.txt", "a");
		fprintf(newFile, "C");
		fclose(newFile);
		ioMutex.release();
	}

    qMutex.release();
	ioMutex.acquire();
	cout << "\n *************   C Finish   *********************";
	newFile = fopen("newFile.txt", "a");
	fprintf(newFile, "\n *************   C Finish   *********************");
	fclose(newFile);
	ioMutex.release();
}

void main()
{
	clrscr();
	SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, RMS);
	SMARTS.declareTask(a, 'A', 3, 402);
	SMARTS.declareTask(b, 'B', 3, 401);
	SMARTS.declareTask(c, 'C', 3, 400);
	SMARTS.runTheTasks();
}

// edf & round robin works with the folowing values:
// i = 100, j = 10 for all the loops.

// edf not work, round robin work with the folowing values:
// no possible

// edf work, round robin not work with the folowing values:
// i = 500000, j = 10 for all the loops.
