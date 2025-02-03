#include "Transform.h"

Transform::Transform(GameObject* parent) : Component(parent)
{
	_position = Vector3(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	_scale = Vector3(1.0f, 1.0f, 1.0f);
}

Transform::~Transform()
{

}
//
//void Transform::SetRotation(Vector3 rotation) 
//{ 
//	if (rotation.x > 0.0f)
//	{
//		rotation.x = 0.0f;
//	}
//	if (rotation.y > 0.0f)
//	{
//		rotation.y = 0.0f;
//	}
//	if (rotation.z > 0.0f)
//	{
//		rotation.z = 0.0f;
//	}
//
//	_orientation = MakeQFromEulerAngles(rotation.x, rotation.y, rotation.z); 
//}
//
//void Transform::SetRotation(float x, float y, float z) 
//{
//	if (x > 0.0f)
//	{
//		x = 0.0f;
//	}
//	if (y > 0.0f)
//	{
//		y = 0.0f;
//	}
//	if (z > 0.0f)
//	{
//		z = 0.0f;
//	}
//	_orientation = MakeQFromEulerAngles(x, y, z); 
//}