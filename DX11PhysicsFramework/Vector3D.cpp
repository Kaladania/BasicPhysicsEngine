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

int Vector3D::CrossProduct(Vector3 a, Vector3 b)
{
	return 0;
}

int Vector3D::GetMagnitude(Vector3 vector)
{
	return 0;
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

Vector3 Vector3D::Normalize(Vector3 vector)
{
	return vector;
}

