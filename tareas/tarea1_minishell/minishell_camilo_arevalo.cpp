/*Juan Camilo Arevalo Arboleda*/
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <fstream>
using namespace std;
int
main(int argc, char *argv[])
{
    string s;
    cout<<"$ ";
    while(1){
        getline(cin,s);
        int rc = fork();
        if (rc < 0) {
            // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (rc == 0) {
            //char* ss=s.c_str();
            cout<<"$ ";
            system(s.c_str());
            break;
        }    
    }
    return 0;
}