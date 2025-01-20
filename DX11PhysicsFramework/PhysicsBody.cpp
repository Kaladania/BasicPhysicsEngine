#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(GameObject* parent, Transform* transform) : Component(parent)
{
	_transform = transform;

	if (transform == nullptr)
	{
		_isActive = false; //disables the component if it doesnt have a valid transform
	}

	_movement = new Movement(parent);
	_movement->SetTransform(transform);
}

PhysicsBody::~PhysicsBody()
{
	delete _movement;
	_movement = nullptr;
}


/// <summary>
/// Updates object's position
/// </summary>
/// <param name="direction">direction to translate position</param>
void PhysicsBody::Move(Vector3 direction)
{
	Vector3 position = _transform->GetPosition();

	position.x += direction.x;
	position.y += direction.y;
	position.z += direction.z;

	_transform->SetPosition(position);
}


void PhysicsBody::SetCollider(ColliderType colliderType)
{
	switch (colliderType)
	{
	case SPHERE_COLLISSION_COMPONENT:

		_collider = new SphereCollider(_parent, _transform);
		break;

	case BOX_COLLISSION_COMPONET:

		_collider = new BoxCollider(_parent, _transform);
		break;

	}
}


/// <summary>
/// Override to update rigidbody physics
/// </summary>
/// <param name="deltaTime"></param>
void PhysicsBody::UpdatePhysics(float deltaTime)
{
	//updates a movement component if it has been added and is active
	if ((_movement != nullptr || _transform != nullptr) && _movement->GetIsActive())
	{
		_movement->Update(deltaTime);
	}
	else
	{

	}
}