#include "DebugOutputer.h"

DebugOutputer::DebugOutputer()
{

}

DebugOutputer::~DebugOutputer()
{

}

/// <summary>
/// Prints the requested string to the Debug Output Console
/// </summary>
/// <param name="string">string to output</param>
void DebugOutputer::PrintDebugString(std::string string)
{
	string += "\n"; //ensures all strings are printed on a new line
	char sz[1024] = { 0 };

	sprintf_s(sz, string.c_str());
	OutputDebugStringA(sz);
}

/// <summary>
/// Prints the requested F-string to the Debug Output Console
/// </summary>
/// <param name="format">formatted string</param>
/// <param name="...">list of parameters to fill in the f-string placeholders</param>
void DebugOutputer::PrintDebugStringF(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);

	const UINT32 MAX_CHARS = 1034;
	static char s_buffer[MAX_CHARS];

	vsnprintf(s_buffer, MAX_CHARS, format, argList);
	OutputDebugStringA(s_buffer);
	va_end(argList);
}