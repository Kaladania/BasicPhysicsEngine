#include "Transform.h"

Transform::Transform()
{
	_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

Transform::~Transform()
{

}