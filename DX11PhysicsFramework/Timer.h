#pragma once
#include <chrono>
#include "DebugOutputer.h"
#define FIXED_DELTA_VALUE 1.0f/60.f //defines the amount of time each frame update should (on average) take

using namespace std::chrono;

//struct FrameworkTimer
//{
//	int timerID = 0; //holds the id of the current framework timer instance
//	float deltaTime = 0.0f; //
//	float timeLimit = 0.0f;
//};

/// <summary>
/// Responsible for managing simulation timer functions/implementation
/// </summary>
class Timer
{
private:

	steady_clock::time_point _lastFrame; //holds a point in time - the 'previous frame'
	DebugOutputer* _debugOutputer = nullptr; //holds a reference to the class's debugger

public:

	Timer();
	~Timer();

	float GetDeltaTime();
	void Tick();
};

