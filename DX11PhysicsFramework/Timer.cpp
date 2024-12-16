#include "Timer.h"

/// <summary>
/// Constructor
/// </summary>
Timer::Timer()
{
	Tick(); //gets the current frame upon initialisation
}

/// <summary>
/// Destructor
/// </summary>
Timer::~Timer()
{
	delete _debugOutputer;
	_debugOutputer = nullptr;
}

/// <summary>
/// Returns the elapsed seconds since last frame
/// </summary>
/// <returns>delta time</returns>
float Timer::GetDeltaTime()
{
	return duration<float>(steady_clock::now() - _lastFrame).count(); //returns the elapsed seconds since last frame
}

/// <summary>
/// Updates frame count to set 'last frame' as current frame
/// </summary>
void Timer::Tick()
{
	//updates frame snapshot 
	_lastFrame = steady_clock::now();
}