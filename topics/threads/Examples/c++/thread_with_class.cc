#include <iostream>
#include <thread>

void foo() { std::cout << "foo()\n"; }
void bar() { std::cout << "bar()\n"; }

class task
{
	public:
	task() { std::cout << "task constructor\n"; }

	void operator()() const
	{
		std::cout << "operator()\n";
		foo();
		bar();
	}
};

int main()
{
	task tsk;
	std::thread t(tsk);
	t.join();
	return 0;
}

//g++ -o thread_with_class thread_with_class.cc -pthread -std=c++0x