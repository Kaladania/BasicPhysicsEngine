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
#pragma region non-manifold code
	////calculates the distance between the center's of the two objects
	//float distance = _vector3D->GetMagnitude(this->_transform->GetPosition() - other->_transform->GetPosition());

	////calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	//float combinedRadii = this->GetCollissionRadius() + other->GetCollissionRadius();

	////if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	//if (distance < combinedRadii)
	//{
	//	return true;
	//}

#pragma endregion

	manifold = CollisionManifold(); //clears the manifold each check


	//gets the distance between the centers of the colliding objects
	Vector3 path = _vector3D->GetMagnitude(other->GetTransform()->GetPosition() - _transform->GetPosition());
	float distance = _vector3D->GetMagnitude(path);

	//gets the combined radius of both spheres
	float sumOfRadii = other->GetCollissionRadius() + _radius;

	//if the distance between centers is less than the combined radii, a collision has occured
	if (distance < sumOfRadii)
	{
		manifold.collisionNormal = _vector3D->Normalize(path); //stores direction of collision
		manifold.contactPointCount = 1; //stores amount of points involved in collission
		manifold.points[0].Position = _transform->GetPosition() + (manifold.collisionNormal * _radius); //stores the position of the point of collision
		manifold.points[0].penetrationDepth = fabsf(distance - sumOfRadii); //stores the amount of overlap involved in the collision

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