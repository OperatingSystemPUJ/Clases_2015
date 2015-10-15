// valgrind --tool=memcheck ./fibonacci

#include <cstdlib>
#include <iostream>

using namespace std;

int main ( );
void f ( int n );

int main ( )
{
  int n = 10;

  cout << "\n";
  cout << "Fibonacci\n";
  cout << "  C++ version.\n";
  cout << "  A sample code for analysis by VALGRIND.\n";

  f ( n );

  cout << "\n";
  cout << "Fibonacci\n";
  cout << "  Normal end of execution.\n";

  return 0;
}

// F computes N+1 entries of the Fibonacci sequence.
// Unfortunately, F only allocates space for N entries.  Hence, the
// assignment of a value to the N+1 entry causes a memory leak.
void f (int n)
{
  int i;
  int *x;

  x = ( int * ) malloc ( n * sizeof ( int ) );

  x[0] = 1;
  cout << "  " << 0 << "  " << x[0] << "\n";

  x[1] = 1;
  cout << "  " << 1 << "  " << x[1] << "\n";

  for ( i = 2; i <= n; i++ )
  {
    x[i] = x[i-1] + x[i-2];
    cout << "  " << i << "  " << x[i] << "\n";
  }

  delete [] x;

  return;
}