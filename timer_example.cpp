#include <iostream>
#include <chrono>
#include <iomanip>


/***************************************************************************
*       Class: Timer
*       Uses:
*           Creates a Timer object with duration based on input
******************************************************************************/

class Timer
{

private:

// variables
std::chrono::time_point<std::chrono::system_clock> start;
double duration;

public:


Timer(double s);
// ~Timer();

// setters
void reset();

// getters
double getTime();
bool isDone();

};

Timer::Timer(double s) : duration(s)
{
    // set starting time
    start = std::chrono::system_clock::now();
}

// ~Timer();

/****************************** Setters *************************************/

// resets timer to current time
void Timer::reset()
{
    start = std::chrono::system_clock::now();
}

/****************************** Getters *************************************/

// returns time that has elapsed since the start of the timer 
double Timer::getTime()
{
    std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - start;
    return elapsed.count();
}

// checks if the timer has elapsed
// true if the timer has finished
// false if the timer hasn't
bool Timer::isDone()
{
    return (getTime() > duration);
}


/****************************** Usage *************************************

double time = 60;
Timer t1(time);
while (!t1.isDone())    // check if timer has elapsed from start time
{
    // do work
    t1.reset(); // special event that maybe resets timer?
    t1.getTime(); // need elapsed time to display or calculation?
}
****************************** Usage ***************************************/

int main()
{
    Timer * gameTime = new Timer(10);

    while (!gameTime->isDone()) {   // displays time every 10th of a second
        if (static_cast<int>(gameTime->getTime()*1000000) % 100000 == 0)
            std::cout << std::showpoint << std::setprecision(3) 
            << gameTime->getTime() << " Timer up? " 
            << std::boolalpha << gameTime->isDone() 
            <<  std::endl;
    }

    if (static_cast<int>(gameTime->getTime()*1000000) % 100000 == 0)
            std::cout << std::showpoint << std::setprecision(2) 
            << gameTime->getTime() << " Timer up? " 
            << std::boolalpha << gameTime->isDone() 
            <<  std::endl;

    delete gameTime;

    return 0;
}