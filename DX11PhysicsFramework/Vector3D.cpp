#include "Vector3D.h"

Vector3D::Vector3D()
{

}
Vector3D::~Vector3D()
{

}

int Vector3D::DotProduct(Vector3 a, Vector3 b)
{
	return 0;
}

Vector3 Vector3D::CrossProduct(Vector3 a, Vector3 b)
{
	Vector3 vector;

	vector.x = (a.y * b.z) - (a.z * b.y);
	vector.y = (a.z * b.x) - (a.x * b.z);
	vector.z = (a.x * b.y) - (a.y * b.x);

	return vector;
}

/// <summary>
/// Calculates the magnitude of a given vector
/// </summary>
/// <param name="vector">desired vector</param>
/// <returns>maginitude of vector</returns>
float Vector3D::GetMagnitude(Vector3 vector)
{
	//calculates the sum of all squares of the co-ordinates
	float squareSum = (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z);

	//returns the magnitude (the square root of the sum)
	return sqrtf(squareSum);
}

/// <summary>
/// Normalizes the given vector
/// </summary>
/// <param name="vector">vector to normalize</param>
/// <returns>resulting unit (normalized) vector</returns>
Vector3 Vector3D::Normalize(Vector3 vector)
{
	float magnitude = GetMagnitude(vector);

	return vector / magnitude;
}


/// <summary>
/// Converts vector to string
/// </summary>
/// <param name="vector">vector to convert</param>
/// <returns>stringed output</returns>
std::string Vector3D::ToString(Vector3 vector)
{
	return "(" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ", " + std::to_string(vector.z) + ")";
}

