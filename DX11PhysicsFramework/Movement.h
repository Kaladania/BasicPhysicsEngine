#pragma once

#include <directxmath.h>
#include <string>
#include "Transform.h"
#include "Vector3D.h"
#include "DebugOutputer.h"

using namespace DirectX;

enum Directions
{
	Left,
	Right,
	Up,
	Down,
	Forwards,
	Backwards
};
/// <summary>
/// Responsible for calculating and applying transformation translations
/// </summary>
class Movement
{
private:

	float _movementSpeed = 0.0f; //stores the current movement speed of the object
	Transform* _transform = nullptr; //stores reference of the transform the component is tied to
	Vector3D* _vector3D = nullptr; //stores a reference to the object's vector3 manager
	DebugOutputer* _debugOutputer = nullptr; //stores a reference to the object's debug outputer

public:

	Movement();
	~Movement();

	void SetMovementSpeed(float speed) { _movementSpeed = speed; }
	void SetTransform(Transform* transform) { _transform = transform; }

	void MoveTransform(Directions direction);
};

