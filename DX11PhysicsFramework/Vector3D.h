#pragma once
#include <string>
//struct resprenting a data type for 3D Vectors
struct Vector3
{
	int x = 0;
	int y = 0;
	int z = 0;

	/// <summary>
	/// + operator overload to add 2 Vector3s
	/// </summary>
	/// <param name="other">other vector to add to current vector</param>
	/// <returns>the sum of the two vectors</returns>
	Vector3 operator+(Vector3 other)
	{
		Vector3 output;

		output.x = this->x + other.x;
		output.y = this->x + other.y;
		output.y = this->x + other.y;

		return output;
	}
};


/// <summary>
/// Responsible for operating and managing 3D Vectors
/// </summary>
class Vector3D
{

private:
	

public:

	Vector3D();
	~Vector3D();

	int DotProduct(Vector3 a = Vector3(), Vector3 b = Vector3());
	int CrossProduct(Vector3 a = Vector3(), Vector3 b = Vector3());
	int GetMagnitude(Vector3 vector = Vector3());
	Vector3 Normalize(Vector3 vector = Vector3());
	std::string ToString(Vector3 vector = Vector3());
};

