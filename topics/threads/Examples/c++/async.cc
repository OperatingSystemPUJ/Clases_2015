#include <thread>
#include <future>
#include <iostream>
#include <chrono>
using namespace std;

int multiply(int a, int b)
{
	return a*b;
}

int main(int argc, char* argv[])
{
	auto a = async(multiply, 2, 3);

	int v = a.get();

	cout << "The thread returned " << v << endl;

	return 0;
}

//g++ -o async async.cc -pthread -std=c++0x

//The template function async runs the function f asynchronously 
//(potentially in a separate thread which may be part of a thread pool) 
//and returns a std::future that will eventually hold the result of that function call. 
