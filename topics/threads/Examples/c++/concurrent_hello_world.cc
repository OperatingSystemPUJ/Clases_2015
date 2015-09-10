#include <iostream>
#include <thread>

void hello()
{
	std::cout<<"Hello Concurrent World\n";
}

int main()
{
	std::thread t(hello);
	t.join();
}


//g++ -o concurrent_hello_world concurrent_hello_world.cc -pthread -std=c++0x
