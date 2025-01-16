#include "Collider.h"

Collider::Collider(GameObject* parent) : Component(parent)
{

}

Collider::~Collider()
{
	_transform = nullptr;
}

bool Collider::CheckForCollission(Collider* other)
{
	bool collided = false;
	if (_isActive && other->GetIsActive())
	{
		collided = this->CollidesWith(other);
		//_debugOutputer->PrintDebugString("COLLISSION!");
	}

	//defaults to exiting out of the function if at least 1 of the colliders are not currently active
	return collided;

}