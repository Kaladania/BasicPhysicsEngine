#include "BoxCollider.h"
#include "SphereCollider.h"
#include <algorithm>

BoxCollider::BoxCollider(GameObject* parent, Transform* transform) : Collider(parent, transform)
{
}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::SetExtents(Vector3 extents)
{
	_halfExtents.x = extents.x * 0.5f;
	_halfExtents.y = extents.y * 0.5f;
	_halfExtents.z = extents.z * 0.5f;

	_center = _transform->GetPosition();
	
	//determines min and max points by finding the point along the object furthest away from center
	_minPoint = Vector3(_center.x - (_halfExtents.x / 0.5f), _center.y - (_halfExtents.y / 0.5), _center.z - (_halfExtents.z / 0.5f));
	_maxPoint = Vector3(_center.x + (_halfExtents.x / 0.5f), _center.y + (_halfExtents.y / 0.5), _center.z + (_halfExtents.z / 0.5f));
}

/// <summary>
/// Checks collission against a default collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool BoxCollider::CollidesWith(Collider* other)
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
bool BoxCollider::CollidesWith(SphereCollider* other)
{
	Vector3 clampedIntersection = Vector3(0, 0, 0);
	Vector3 sphereCenter = other->GetTransform()->GetPosition();

	////determines min and max points by finding the point along the object furthest away from center
	//_minPoint = Vector3(_center.x - _halfExtents.x, _center.y - _halfExtents.y, _center.z - _halfExtents.z);
	//_maxPoint = Vector3(_center.x + _halfExtents.x, _center.y + _halfExtents.y, _center.z + _halfExtents.z);

	//Vector3 sphereCenter = other->GetTransform()->GetPosition();

	//float totalSquareDistance = 0.0f;

	////gets the total square distance between the center of the sphere and the minimum/maximum points of the AABB
	//totalSquareDistance += GetSquareAxisDistance(sphereCenter.x, _minPoint.x, _maxPoint.x);
	//_debugOutputer->PrintDebugString("(After X) Total Distance is now : " + std::to_string(totalSquareDistance));
	//totalSquareDistance += GetSquareAxisDistance(sphereCenter.y, _minPoint.y, _maxPoint.y);
	//_debugOutputer->PrintDebugString("(After Y) Total Distance is now : " + std::to_string(totalSquareDistance));
	//totalSquareDistance += GetSquareAxisDistance(sphereCenter.z, _minPoint.z, _maxPoint.z);
	//_debugOutputer->PrintDebugString("(After Z) Total Distance is now : " + std::to_string(totalSquareDistance));

	////cache's the sphere's radius
	//float sphereRadius = other->GetCollissionRadius();

	//_debugOutputer->PrintDebugString("Squared sphere Radius is: " + std::to_string(sphereRadius * sphereRadius));

	//if (totalSquareDistance <= (sphereRadius * sphereRadius))
	//{
	//	return true;
	//}

	clampedIntersection.x = Clamp(sphereCenter.x, _minPoint.x, _maxPoint.x);
	clampedIntersection.y = Clamp(sphereCenter.y, _minPoint.y, _maxPoint.y);
	clampedIntersection.z = Clamp(sphereCenter.z, _minPoint.z, _maxPoint.z);

	//cache's the sphere's radius
	float sphereRadius = other->GetCollissionRadius();

	float distance = clampedIntersection.x - sphereCenter.x;

	bool collided = false;
	//distance = sqrtf(distance * distance);

	//calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	//float combinedRadii = this->_halfExtents.x + other->GetExtents().x;

	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < sphereRadius)
	{
		collided = true;
	}
	else
	{
		return false; //immediately breaks out of the function and returns a failed collision check
	}

	distance = clampedIntersection.y - sphereCenter.y;

	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < sphereRadius)
	{
		collided = true;
	}
	else
	{
		return false; //immediately breaks out of the function and returns a failed collision check
	}

	distance = clampedIntersection.z - sphereCenter.z;

	//calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < sphereRadius)
	{
		collided = true;
	}
	else
	{
		return false; //immediately breaks out of the function and returns a failed collision check
	}

	_collisionManifold = CollisionManifold();

	_collisionManifold.collisionNormal = _vector3D->Normalize(this->_transform->GetPosition() - other->GetTransform()->GetPosition()); //stores direction of collision
	_collisionManifold.contactPointCount = 1; //stores amount of points involved in collission
	_collisionManifold.points[0].Position = _transform->GetPosition() + (_collisionManifold.collisionNormal * other->GetCollissionRadius()); //stores the position of the point of collision

	Vector3 otherPosition = other->GetTransform()->GetPosition();
	Vector3 position = _transform->GetPosition();

	float xOverlap = otherPosition.x - position.x;
	float yOverlap = otherPosition.y - position.y;
	float zOverlap = otherPosition.z - position.z;

	_collisionManifold.points[0].penetrationDepth = fabsf(_vector3D->GetMagnitude(this->_transform->GetPosition() - otherPosition) - (_halfExtents.x + sphereRadius)); //stores the smallest axis penetration as the penetration depth

	return true;

	//returns a bool stating if the two shapes are colliding
	//collission occures if the distance between sphere center and AABB (totalSquareDistance) is less than the squared radius
	//return false;
}

/// <summary>
/// Uses the SAT to get the distance between two shapes on a parrallel axis
/// </summary>
/// <param name="aMin">Minimum point of shape A on a parrallel axis</param>
/// <param name="aMax">Maximum point of shape A on a parrallel axis</param>
/// <param name="bMin">Minimum point of shape B on a parrallel axis</param>
/// <param name="bMax">Maximum point of shape B on a parrallel axis</param>
/// <returns>The distance between the two shapes on a parrallel axis</returns>
float CalculateDistanceBetweenAxis(float aMin = 0.0f, float aMax = 0.0f, float bMin = 0.0f, float bMax = 0.0f)
{
	return 0;
}

/// <summary>
/// Returns a clamped value between a given sphere center and the extents of this collider's AABB
/// </summary>
/// <param name="sphereCenter">The center of the colliding sphere</param>
/// <returns>The clamped value</returns>
float BoxCollider::Clamp(float value, float min, float max)
{
	if (value < min)
	{
		return min; //returns the minimum if the given value is under the range
	}
	else if (value > max)
	{
		return max; //returns the max if the given value goes over the range
	}

	return value; //returns the value if it's within range
}

/// <summary>
/// Returns the squared distance between two co-ordinate values on the same axis
/// </summary>
/// <param name="a">first point</param>
/// <param name="b">second point</param>
/// <returns>squared distance</returns>
float BoxCollider::GetSquareAxisDistance(float sphereAxisValue, float AABBAxisValueMin, float AABBAxisValueMax)
{
	float squareDistance = 0.0f;

	//if the sphere value of the current axis is less than the AABB value (e.g if sphere.x < AABB.x)
	if (sphereAxisValue < AABBAxisValueMin)
	{
		//gets the excess amount of sphere space beyond the AABB's minimum point
		squareDistance = (AABBAxisValueMin - sphereAxisValue) * (AABBAxisValueMin - sphereAxisValue);
	}
	//if the sphere value of the current axis is more than the AABB value (e.g if sphere.y > AABB.y)
	else if (sphereAxisValue > AABBAxisValueMax)
	{
		//gets the excess amount of sphere space beyond the AABB's maximum point
		squareDistance = (sphereAxisValue - AABBAxisValueMax) * (sphereAxisValue - AABBAxisValueMax);
	}

	return squareDistance;
}

/// <summary>
/// Checks collission against another sphere collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool BoxCollider::CollidesWith(BoxCollider* other)
{
	bool collided = false;

	//calculates the distance between the center's of the two objects
	float distance = _vector3D->GetMagnitude(other->_transform->GetPosition() - this->_transform->GetPosition());

	//determines min and max points by finding the point along the object furthest away from center
	_minPoint = Vector3(_center.x - _halfExtents.x, _center.y - _halfExtents.y, _center.z -_halfExtents.z);
	_maxPoint = Vector3(_center.x + _halfExtents.x, _center.y + _halfExtents.y, _center.z +_halfExtents.z);

	distance = this->_transform->GetPosition().x - other->_transform->GetPosition().x;
	distance = sqrtf(distance * distance);

	//calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	float combinedRadii = this->_halfExtents.x + other->GetExtents().x;

	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < combinedRadii)
	{
		collided = true;
	}
	else
	{
		return false; //immediately breaks out of the function and returns a failed collision check
	}

	distance = this->_transform->GetPosition().y - other->_transform->GetPosition().y;
	distance = sqrtf(distance * distance);

	//calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	combinedRadii = this->_halfExtents.y + other->GetExtents().y;

	if (distance < combinedRadii)
	{
		collided = true;
	}
	else
	{
		return false;
	}

	distance = this->_transform->GetPosition().z - other->_transform->GetPosition().z;
	distance = sqrtf(distance * distance);

	//calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	combinedRadii = this->_halfExtents.z + other->GetExtents().z;

	if (distance < combinedRadii)
	{
		collided = true;
	}
	else
	{
		return false; 
	}

	_collisionManifold = CollisionManifold();

	Vector3 otherCenter = other->GetTransform()->GetPosition();
	Vector3 otherExtents = other->GetExtents();

	Vector3 otherMinPoint = Vector3(otherCenter.x - otherExtents.x, otherCenter.y - otherExtents.y, otherCenter.z - otherExtents.z);
	Vector3 otherMaxPoint = Vector3(otherCenter.x + otherExtents.x, otherCenter.y + otherExtents.y, otherCenter.z + otherExtents.z);

	_collisionManifold.collisionNormal = _vector3D->Normalize(this->_transform->GetPosition() - other->GetTransform()->GetPosition()); //stores direction of collision
	_collisionManifold.contactPointCount = 1; //stores amount of points involved in collission
	_collisionManifold.points[0].Position = Vector3(); //stores the position of the point of collision //DEFAULT VALUE, CHECK INTEGRITY

	Vector3 otherPosition = other->GetTransform()->GetPosition();
	Vector3 position = _transform->GetPosition();

	Vector3 clampedIntersection = Vector3();

	float xOverlap = fabsf(otherMinPoint.x - _maxPoint.x);
	float yOverlap = fabsf(otherMinPoint.y - _maxPoint.y);
	float zOverlap = fabsf(otherMinPoint.z - _maxPoint.z);

	//_collisionManifold.points[0].penetrationDepth = min(xOverlap, yOverlap, zOverlap); //stores the smallest axis penetration as the penetration depth


	_collisionManifold.points[0].penetrationDepth = fabsf(_vector3D->GetMagnitude(this->_transform->GetPosition() - otherCenter) - combinedRadii); //stores the smallest axis penetration as the penetration depth

	return true;
}

bool BoxCollider::CollidesWith(PlaneCollider* other)
{
	return false;
}
