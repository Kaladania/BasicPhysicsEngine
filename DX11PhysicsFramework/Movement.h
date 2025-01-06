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
	Vector3 _netForce = Vector3(); //holds the total amount of force being applied to the parent object

	float _mass = 1.0f; //holds the current mass of the parent object

	bool _isAutomatic = false;
	bool _needsToMove = false;

	bool _hasConstantVelocity = false; //states if the parent object is moving at a constant velocity
	bool _hasConstantAcceleration = false; //states if the parent object is accelerating at a constant rate


public:

	Movement(GameObject* parent = nullptr);
	~Movement();

	//sets new values for movement variables to change parent object speed

	void SetMovementSpeed(float speed) { _movementSpeed = speed; }
	void SetTransform(Transform* transform) { _transform = transform; }
	void SetVelocity(Vector3 velocity) { _velocity = velocity; }
	void SetAcceleration(Vector3 acceleration) { _acceleration = acceleration; }
	

	void MoveTransform(Directions direction);
	void AddForce(Vector3 force) { _netForce += force; } //adds a force to the object

	Vector3 CalculateDisplacement(Vector3 displacement = Vector3(), float deltaTime = 0.0f);
	virtual void Update(float deltaTime);
};

