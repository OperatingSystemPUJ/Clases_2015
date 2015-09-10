#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void print_string(const char *s, int sleepms)
{
	while(1)
	{
		cout << s << endl;
		this_thread::sleep_for(chrono::milliseconds(sleepms));
	}
}

int main(int argc, char* argv[])
{
	thread t1(print_string, "0", 1);
	thread t2(print_string, "1", 2);

	t1.join();
	t2.join();

	return 0;
}

//g++ -o thread_with_params thread_with_params.cc -pthread -std=c++0x