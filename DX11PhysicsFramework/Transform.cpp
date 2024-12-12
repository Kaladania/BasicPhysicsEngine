#include "Transform.h"

Transform::Transform(GameObject* parent) : Component(parent)
{
	_position = Vector3(0.0f, 0.0f, 0.0f);
	_rotation = Vector3(0.0f, 0.0f, 0.0f);
	_scale = Vector3(1.0f, 1.0f, 1.0f);

	_debugOutputer->PrintDebugString(_vector3D->ToString(_vector3D->Normalize(Vector3(2, 3, 4))));
}

Transform::~Transform()
{

}