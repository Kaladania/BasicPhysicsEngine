#include "DebugOutputer.h"

DebugOutputer::DebugOutputer()
{

}

DebugOutputer::~DebugOutputer()
{

}

void DebugOutputer::PrintDebugString(std::string string)
{
	string += "\n"; //ensures all strings are printed on a new line
	char sz[1024] = { 0 };
	//sprintf_s();
	sprintf_s(sz, string.c_str());
	OutputDebugStringA(sz);
}