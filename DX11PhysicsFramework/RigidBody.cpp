#include "RigidBody.h"

RigidBody::RigidBody(GameObject* parent, Transform* transform) : PhysicsBody(parent, transform)
{

}

RigidBody::~RigidBody()
{
	
	delete _collider;

	
	_collider = nullptr;
}

/// <summary>
/// Override to update rigidbody physics
/// </summary>
/// <param name="deltaTime"></param>
void RigidBody::UpdatePhysics(float deltaTime)
{
	//updates a movement component if it has been added and is active
	if (_movement != nullptr && _movement->GetIsActive())
	{
		_movement->Update(deltaTime);
	}
}

