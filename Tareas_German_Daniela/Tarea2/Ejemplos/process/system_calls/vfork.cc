#include <iostream>
#include <string>
#include <cstdlib>

// Required by for routine
#include <sys/types.h>
#include <unistd.h>	 

using namespace std;
	 
int globalVariable = 2;
	 
main()
{
	string sIdentifier;
	int    iStackVariable = 20;
 
 	/*
 	 * the new process created using vfork() system call executes as a thread in the parent's address 
 	 * space and until the child thread doesnot calls exit() or exec() system call, the parent is blocked. 
 	 */
	pid_t pID = vfork();  
	
	if (pID == 0)                // child
	{
		// Code only executed by child process

		sIdentifier = "Child Process: ";
		globalVariable++;
		iStackVariable++;
		cout << sIdentifier;
		cout << " Global variable: " << globalVariable;
		cout << " Stack variable: "  << iStackVariable << endl;
		exit(0);
	}
	else if (pID < 0)            // failed to fork
	{
		cerr << "Failed to fork" << endl;
		exit(1);
		// Throw exception
	}
	else                                   // parent
	{
		// Code only executed by parent process
	
		sIdentifier = "Parent Process:";
	} 
	// executed only by parent

	cout << sIdentifier;
	cout << " Global variable: " << globalVariable;
	cout << " Stack variable: "  << iStackVariable << endl;
	exit(0);
}