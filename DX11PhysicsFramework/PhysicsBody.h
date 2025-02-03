#pragma once

#include "Movement.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

#include "Component.h"

enum PhysicsType
{
	NOPHYSICS,
	RIGIDBODY_PHYSICS,
	PARTICLE_PHYSICS,
};

/// <summary>
/// Base class for physics body types
/// </summary>
class PhysicsBody abstract : public Component
{
protected:

	Movement* _movement = nullptr; //stores a reference to the object's movement component
	Transform* _transform = nullptr; //stores a reference to the object's transform information
	Collider* _collider = nullptr; //stores a reference to the object's collider component

	//DebugOutputer* _debugOutputer = nullptr; //stores a reference to the object's debug outputer

public:
	PhysicsBody(GameObject* parent = nullptr, Transform* transform = nullptr);
	~PhysicsBody();

	//void AddPhysicsBody(PhysicsType bodyType);

	Movement* GetMovement() const { return _movement; }

	void SetCollider(ColliderType colliderType);
	Collider* GetCollider() const { return _collider; }

	virtual void UpdatePhysics(float deltaTime = 0.0f) ;
	void Move(Vector3 direction);

	//virtual void AddRelativeForce(Vector3 force, Vector3 originPoint);
};

