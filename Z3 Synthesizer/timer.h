//Written by Harutyun Rehanyan
//add cont function
#ifndef TIMER_H
#define TIMER_H
#include <chrono>
#include <iomanip>

typedef std::chrono::high_resolution_clock::time_point TIME_POINT;

class timer
{
private:
	TIME_POINT t1, t2;
	bool STOPPED = false;
public:
	void begin()
	{
		t1 = std::chrono::high_resolution_clock::now();
		STOPPED = false;
	}
	void end()
	{		
		t2 = std::chrono::high_resolution_clock::now();
		STOPPED = true;
	}
	double elapsed()
	{
		if (STOPPED)
			return std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
		else
			end();
		return std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
	}
	void print_elapsed()
	{
		std::cout << "Time elapsed: " << std::fixed << elapsed() << " seconds" << std::endl;
	}
	void print_elapsed(int i)
	{
		std::cout << "Time elapsed: " << std::fixed << std::setprecision(i) << elapsed() << " seconds" << std::endl;
	}
};
#endif

/*
void cont()
{
t1 = t2 - t1;
t2 = std::chrono::high_resolution_clock::now();
}
bool CONT = false;
*/