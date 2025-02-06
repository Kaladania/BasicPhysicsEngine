#include "PlaneCollider.h"



PlaneCollider::PlaneCollider(GameObject* parent)
{
}

PlaneCollider::~PlaneCollider()
{

}

/// <summary>
/// Checks collission against a default collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool PlaneCollider::CollidesWith(Collider* other, CollisionManifold& manifold)
{
	if (_isActive)
	{
		return other->CollidesWith(this, _collisionManifold);
	}

	//defaults to no collission if component is not active
	return false;
}

/// <summary>
/// Checks collission against another sphere collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool PlaneCollider::CollidesWith(SphereCollider* other, CollisionManifold& manifold)
{
	////calculates the distance between the center's of the two objects
	//float distance = _vector3D->GetMagnitude(this->_transform->GetPosition() - other->_transform->GetPosition());
	//_debugOutputer->PrintDebugString("This object's center: " + _vector3D->ToString(this->_transform->GetPosition()) + "Other Object's center: " + _vector3D->ToString(other->_transform->GetPosition()) + ". Distance is: " + std::to_string(distance));

	////calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	//float combinedRadii = this->GetCollissionRadius() + other->GetCollissionRadius();
	//_debugOutputer->PrintDebugString("Combined Radii is: " + std::to_string(combinedRadii));

	////if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	//if (distance < combinedRadii)
	//{
	//	return true;
	//}

	return false;
}

/// <summary>
/// Checks collission against                      another sphere collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool PlaneCollider::CollidesWith(BoxCollider* other, CollisionManifold& manifold)
{
	Vector3 objectDirection = other->GetTransform()->GetPosition();

	return false;
}

/// <summary>
/// Checks collission against another sphere collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool PlaneCollider::CollidesWith(PlaneCollider* other, CollisionManifold& manifold)
{
	return false;
}