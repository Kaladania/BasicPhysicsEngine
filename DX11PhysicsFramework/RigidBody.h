#pragma once
#include "PhysicsBody.h"


//enum ColliderType
//{
//	NOCOLLIDER,
//	SPHERE_COLLIDER,
//	BOX_COLLIDER,
//};

/// <summary>
/// Physics body subclass responsible for handling solid object physics
/// </summary>
class RigidBody : public PhysicsBody
{
private:
	
	

public:

	RigidBody(GameObject* parent, Transform* transform = nullptr);
	~RigidBody();

	void UpdatePhysics(float deltaTime = 0.0f) override;
};

