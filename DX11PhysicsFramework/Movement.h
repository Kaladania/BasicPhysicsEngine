#pragma once

#include <directxmath.h>
#include <string>
#include "Transform.h"
#include "Component.h"

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
class Movement : public Component
{
private:

	float _movementSpeed = 0.0f; //stores the current movement speed of the object
	
	
protected:

	Transform* _transform = nullptr; //stores reference of the transform the component is tied to
	Vector3 _velocity = Vector3(); //holds the current velocity of the transform
	Vector3 _acceleration = Vector3(); //holds the current acceleration of the transform

	bool _isAutomatic = false;
	bool _needsToMove = false;


public:

	Movement(GameObject* parent = nullptr);
	~Movement();

	void SetMovementSpeed(float speed) { _movementSpeed = speed; }
	void SetTransform(Transform* transform) { _transform = transform; }
	void SetVelocity(Vector3 velocity) { _velocity = velocity; }
	void SetAcceleration(Vector3 acceleration) { _acceleration = acceleration; }
	

	void MoveTransform(Directions direction);

	Vector3 CalculateDisplacement(Vector3 displacement = Vector3(), float deltaTime = 0.0f);
	virtual void Update(float deltaTime);
};

