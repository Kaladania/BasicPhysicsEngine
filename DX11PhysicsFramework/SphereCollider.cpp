#include "SphereCollider.h"

SphereCollider::SphereCollider(GameObject* parent)
{
}

/// <summary>
/// Checks collission against a default collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool SphereCollider::CollidesWith(Collider* other)
{
	if (_isActive)
	{
		return other->CollidesWith(this);
	}

	//defaults to no collission if component is not active
	return false;
}

/// <summary>
/// Checks collission against another sphere collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool SphereCollider::CollidesWith(SphereCollider* other)
{

}