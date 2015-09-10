#include <iostream>
#include <thread>

static const int num_threads = 10;

//This function will be called from a thread

void call_from_thread(int tid) {
	std::cout << "thread " << tid << std::endl;
}
	
int main() {
	std::thread t[num_threads];

	//Launch a group of threads
	for (int i = 0; i < num_threads; ++i) {
		t[i] = std::thread(call_from_thread, i);
	}

	std::cout << "Launched from the main\n";

	//Join the threads with the main thread
	for (int i = 0; i < num_threads; ++i) {
		t[i].join();
	}

	return 0;
}

//g++ -o many_threads many_threads.cc -pthread -std=c++0x