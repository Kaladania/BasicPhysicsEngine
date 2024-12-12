#pragma once
#include <string>
#include <Windows.h>

/// <summary>
/// Responsible for outputting debug strings to the debug output
/// </summary>
class DebugOutputer
{
private:

public:

	DebugOutputer();
	~DebugOutputer();

	void PrintDebugString(std::string string = "");
};

