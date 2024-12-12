#include "Transform.h"

Transform::Transform(GameObject* parent) : Component(parent)
{
	_position = Vector3(0.0f, 0.0f, 0.0f);
	_rotation = Vector3(0.0f, 0.0f, 0.0f);
	_scale = Vector3(1.0f, 1.0f, 1.0f);

	_debugOutputer->PrintDebugString(std::to_string(_vector3D->GetMagnitude(Vector3(5, 2, 7))));
}

Transform::~Transform()
{

}