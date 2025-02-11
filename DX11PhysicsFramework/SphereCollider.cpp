#include "SphereCollider.h"
#include "BoxCollider.h"

SphereCollider::SphereCollider(GameObject* parent, Transform* transform) : Collider(parent, transform)
{
}

SphereCollider::~SphereCollider()
{

}

/// <summary>
/// Checks collission against a default collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool SphereCollider::CollidesWith(Collider* other, CollisionManifold& manifold)
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
bool SphereCollider::CollidesWith(SphereCollider* other, CollisionManifold& manifold)
{
	//calculates the distance between the center's of the two objects
	float distance = _vector3D->GetMagnitude(this->_transform->GetPosition() - other->_transform->GetPosition());
	//_debugOutputer->PrintDebugString("This object's center: " + _vector3D->ToString(this->_transform->GetPosition()) + "Other Object's center: " + _vector3D->ToString(other->_transform->GetPosition()) + ". Distance is: " + std::to_string(distance));

	//calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	float combinedRadii = this->GetCollissionRadius() + other->GetCollissionRadius();
	//_debugOutputer->PrintDebugString("Combined Radii is: " + std::to_string(combinedRadii));

	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < combinedRadii)
	{
		return true;
	}
	
	return false;
}

/// <summary>
/// Checks collission against another sphere collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool SphereCollider::CollidesWith(BoxCollider* other, CollisionManifold& manifold)
{
	//Vector3 clampedIntersection = Vector3(0, 0, 0);

	//Vector3 halfExtents = other->GetExtents();
	//Vector3 boxCenter = other->GetTransform()->GetPosition();

	////determines min and max points by finding the point along the object furthest away from center
	//Vector3 _minPoint = Vector3(boxCenter.x - halfExtents.x, boxCenter.y - halfExtents.y, boxCenter.z - halfExtents.z);
	//Vector3 _maxPoint = Vector3(boxCenter.x + halfExtents.x, boxCenter.y + halfExtents.y, boxCenter.z + halfExtents.z);

	//Vector3 sphereCenter = other->GetTransform()->GetPosition();

	//float totalSquareDistance = 0.0f;

	////gets the total square distance between the center of the sphere and the minimum/maximum points of the AABB
	//totalSquareDistance += other->GetSquareAxisDistance(sphereCenter.x, _minPoint.x, _maxPoint.x);
	//_debugOutputer->PrintDebugString("(After X) Total Distance is now : " + std::to_string(totalSquareDistance));
	//totalSquareDistance += other->GetSquareAxisDistance(sphereCenter.y, _minPoint.y, _maxPoint.y);
	//_debugOutputer->PrintDebugString("(After Y) Total Distance is now : " + std::to_string(totalSquareDistance));
	//totalSquareDistance += other->GetSquareAxisDistance(sphereCenter.z, _minPoint.z, _maxPoint.z);
	//_debugOutputer->PrintDebugString("(After Z) Total Distance is now : " + std::to_string(totalSquareDistance));

	////cache's the sphere's radius
	////float sphereRadius = other->GetCollissionRadius();

	//_debugOutputer->PrintDebugString("Squared sphere Radius is: " + std::to_string(_radius * _radius));

	//if (totalSquareDistance <= (_radius * _radius))
	//{
	//	return true;
	//}

	//returns a bool stating if the two shapes are colliding
	//collission occures if the distance between sphere center and AABB (totalSquareDistance) is less than the squared radius
	//return false;

	Vector3 halfExtents = other->GetExtents();
	Vector3 boxCenter = other->GetTransform()->GetPosition();

	Vector3 _minPoint = Vector3(boxCenter.x - halfExtents.x, boxCenter.y - halfExtents.y, boxCenter.z - halfExtents.z);
	Vector3 _maxPoint = Vector3(boxCenter.x + halfExtents.x, boxCenter.y + halfExtents.y, boxCenter.z + halfExtents.z);

	Vector3 clampedIntersection = Vector3(0, 0, 0);

	Vector3 center = _transform->GetPosition();

	clampedIntersection.x = other->Clamp(center.x, _minPoint.x, _maxPoint.x);
	clampedIntersection.y = other->Clamp(center.y, _minPoint.y, _maxPoint.y);
	clampedIntersection.z = other->Clamp(center.z, _minPoint.z, _maxPoint.z);

	float distance = clampedIntersection.x - center.x;

	bool collided = false;
	//distance = sqrtf(distance * distance);

	//calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	//float combinedRadii = this->_halfExtents.x + other->GetExtents().x;

	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < _radius)
	{
		collided = true;
	}
	else
	{
		return false; //immediately breaks out of the function and returns a failed collision check
	}

	distance = clampedIntersection.y - center.y;

	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < _radius)
	{
		collided = true;
	}
	else
	{
		return false; //immediately breaks out of the function and returns a failed collision check
	}

	distance = clampedIntersection.z - center.z;

	//calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < _radius)
	{
		collided = true;
	}
	else
	{
		return false; //immediately breaks out of the function and returns a failed collision check
	}

	return true;
}

bool SphereCollider::CollidesWith(PlaneCollider* other, CollisionManifold& manifold)
{
	return false;
}