#include <iostream>
02	#include <string>
03	 
04	// Required by for routine
05	#include <sys/types.h>
06	#include <unistd.h>
07	 
08	using namespace std;
09	 
10	 
11	int globalVariable = 2;
12	 
13	main()
14	{
15	   string sIdentifier;
16	   int    iStackVariable = 20;
17	 
18	   pid_t pID = vfork();
19	   if (pID == 0)                // child
20	   {
21	      // Code only executed by child process
22	 
23	      sIdentifier = "Child Process: ";
24	      globalVariable++;
25	      iStackVariable++;
26	      cout << sIdentifier;
27	      cout << " Global variable: " << globalVariable;
28	      cout << " Stack variable: "  << iStackVariable << endl;
29	      _exit(0);
30	    }
31	    else if (pID < 0)            // failed to fork
32	    {
33	        cerr << "Failed to fork" << endl;
34	        exit(1);
35	        // Throw exception
36	    }
37	    else                                   // parent
38	    {
39	      // Code only executed by parent process
40	 
41	      sIdentifier = "Parent Process:";
42	    }
43	 
44	    // executed only by parent
45	 
46	    cout << sIdentifier;
47	    cout << " Global variable: " << globalVariable;
48	    cout << " Stack variable: "  << iStackVariable << endl;
49	    exit(0);
50	}