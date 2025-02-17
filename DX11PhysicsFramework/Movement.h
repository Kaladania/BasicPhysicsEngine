#pragma once

#include <directxmath.h>
#include <string>
#include "Transform.h"
#include "Component.h"

#include "Collider.h"

#define DENSITY_OF_FLUID 1.0f
//#define CROSS_SECTIONAL_AREA 4.0f //(for cubes, 2f * 2f = 4f

#define FRICTION_COEFFICIENT 1.3f //determines rate of de-celeration due to friction (the lower the value, the slower the object decelerates - makes it more 'slippy'
#define STATIC_FRICTION_COEFFICIENT 2.0f //determines rate of de-celeration due to friction (the lower the value, the slower the object decelerates - makes it more 'slippy'
#define ANGULAR_DAMPING 0.5f

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
	
	Transform* _transform = nullptr; //stores reference of the transform the component is tied to
	Vector3 _velocity = Vector3(); //holds the current velocity of the transform
	Vector3 _acceleration = Vector3(); //holds the current acceleration of the transform
	Vector3 _netForce = Vector3(); //holds the total amount of force being applied to the parent object
	Vector3 _gravity = Vector3(); //holds the amount of downward force being exherted by gravity
	Vector3 _oldVelocity = Vector3(); //holds the amount of drag being applied to the parent object

	float _mass = 1.0f; //holds the current mass of the parent object

	bool _isSimulatingGravity = false; //states if the object is being influenced by gravity
	bool _isStationary = false; //states if the object is currently supposed to be stationary

	bool _usesForcedFloor = false; //states if the object forceably stops at 'floor-level'

	bool _hasConstantVelocity = false; //states if the parent object is moving at a constant velocity
	bool _hasConstantAcceleration = false; //states if the parent object is accelerating at a constant rate

	bool _isColliding = false; //states if the object is currently colliding (REPLACE WITH A FUNCTION CALL TO THE COLLIDER)

	float _dragCoefficient = 0.04f; //holds the current drag co-efficient. Defaults to 0.04f for a generic streamlined body
	float _crossSectionalArea = 0.0f; //holds the current drag co-efficient. Defaults to 0.04f for a generic streamlined body
	float _COR = 0.0f; //holds the current co-efficient of resitution

	Vector3 _angularVelocity = Vector3(0, 0, 0);

	Vector3 _torque = Vector3(0, 0, 0); //holds the amount of torque currently being applied to the object
	XMFLOAT3X3 _inertiaTensorMatrix;


public:

	Movement(GameObject* parent = nullptr);
	~Movement();

	//sets new values for movement variables to change parent object speed

	void SetMovementSpeed(float speed) { _movementSpeed = speed; }
	void SetTransform(Transform* transform) { _transform = transform; }
	Transform* GetTransform() const { return _transform; }
	void SetVelocity(Vector3 velocity) { _velocity = velocity; }
	Vector3 GetVeclocity() const { return _velocity; }
	void SetMass(float mass) { _mass = mass; }

	void SetCOR(float COR);
	float GetCOR() const { return _COR; }

	void SetCrossSectionalArea(float area) { _crossSectionalArea = area; }
	//float GetCrossSectionalArea() const { return _crossSectionalArea; }

	float GetMass() const { return _mass; }
	float GetInverseMass();
	void SetAcceleration(Vector3 acceleration) { _acceleration = acceleration; }

	void SetDragCoefficient(float dragCoefficient) { _dragCoefficient = dragCoefficient; }

	void SetIsSimulatingGravity(bool simulationState) { _isSimulatingGravity = simulationState; }
	void SetIsUsingFloor(bool floorState) { _usesForcedFloor = floorState; }

	void SetIsStationary(bool stationaryState) { _isStationary = stationaryState; }
	bool GetIsStationary(bool stationaryState) { return _isStationary; }

	float GetCOR() { return _COR; } //returns the object's COR to use in collision resolution calculations
	

	void MoveTransform(Directions direction);
	void AddForce(Vector3 force) { _netForce += force; } //adds a force to the object
	void ApplyImpulse(Vector3 impulse = Vector3());


	void AddRelativeForce(Vector3 force, Vector3 originPoint);

	//overloads to set the InertiaMatrix
	void SetInertiaMatrix(Vector3 halfExtents);
	void SetInertiaMatrix(float radius);

	Vector3 CalculateDisplacement(Vector3 displacement = Vector3(), float deltaTime = 0.0f);
	Vector3 CalculateDragForce();
	Vector3 CalulateFrictionForce();
	void CalculateCollisionResolutionForce(const float otherCOR);
	void CalculateImpulse(Movement* otherMovement = nullptr, CollisionManifold otherManifold = CollisionManifold());
	void CalculateAngularMovement(float deltaTime);

	virtual void Update(float deltaTime);
};

