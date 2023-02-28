#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
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

bool paused;
Timer * pause_timer;
double pause_duration;

public:

Timer();    // used to create a timer that counts up or dc about it expiring
            // note: the isDone() function is pretty useless and you should
            //      be looking at getTime to see how long the timer has gone
            //      for
Timer(double s);    // used to create a timer that counts down and will tell
                    // you if it expires
~Timer();

// setters
void reset();
void pause();
void unPause();

// getters
double getTime();
bool isDone();
bool isPaused();

};

Timer::Timer() : duration(-1), pause_duration(0.00), 
                pause_timer(nullptr), paused(false)
{    // set starting time
    start = std::chrono::system_clock::now();
}

Timer::Timer(double s) : duration(s), pause_duration(0.00), 
                pause_timer(nullptr), paused(false)
{    // set starting time
    start = std::chrono::system_clock::now();
}

// delete pause timer if it were active
Timer::~Timer()
{
    if (pause_timer)
        delete pause_timer;
}

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
    double net_time = 0;
    std::chrono::duration<double> total_elapsed = std::chrono::system_clock::now() - start;

    if (paused)
    {
        net_time = (total_elapsed.count() - pause_duration - pause_timer->getTime());
    } else {
        net_time = (total_elapsed.count()-pause_duration);
    }

    return net_time;

}

// checks if the timer has elapsed
// true if the timer has finished
// false if the timer hasn't
bool Timer::isDone()
{
    if (duration == -1) {   // return false for count up timers
        return false;
    } else {    // return net time for countdown timers
        return (getTime() > duration);  
    }   
}

void Timer::pause()
{
    paused = true;
    pause_timer = new Timer();
}

bool Timer::isPaused()
{
    return paused;
}

void Timer::unPause()
{
    paused = false;
    pause_duration += pause_timer->getTime();
    delete pause_timer;
    pause_timer = nullptr;
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

void pause_wrapper(Timer * t)
{

}

int main()
{
    Timer * gameTime = new Timer(20);
    int num;

    while (!gameTime->isDone()) {  
        
        if (cin.get()) {
            if (!gameTime->isPaused()) {
                gameTime->pause();
                cout << "Timer paused @ " << showpoint << setprecision(3) 
                        << gameTime->getTime() << endl;
                cout << "\tIs Timer done? " << boolalpha << gameTime->isDone() << endl;
            } else if (gameTime->isPaused()) {
                gameTime->unPause();
                cout << "Timer unpaused @ " << showpoint << setprecision(3) 
                        << gameTime->getTime() << endl;
                cout << "\tIs Timer done? " << boolalpha << gameTime->isDone() << endl;
            }

        }

    }

    std::cout << std::showpoint << std::setprecision(2) 
            << gameTime->getTime() << " Timer up? " 
            << std::boolalpha << gameTime->isDone() 
            <<  std::endl;

    delete gameTime;

    return 0;
}