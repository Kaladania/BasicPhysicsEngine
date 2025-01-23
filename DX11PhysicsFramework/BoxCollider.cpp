#include "BoxCollider.h"


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
/// Checks collission against another sphere collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool BoxCollider::CollidesWith(BoxCollider* other)
{
	bool collided = false;

	//_center = _transform->GetPosition();

	//calculates the distance between the center's of the two objects
	float distance = _vector3D->GetMagnitude(other->_transform->GetPosition() - this->_transform->GetPosition());
	//_debugOutputer->PrintDebugString("This object's center: " + _vector3D->ToString(this->_transform->GetPosition()) + "Other Object's center: " + _vector3D->ToString(other->_transform->GetPosition()) + ". Distance is: " + std::to_string(distance));


	//determines min and max points by finding the point along the object furthest away from center
	_minPoint = Vector3(_center.x - _halfExtents.x, _center.y - _halfExtents.y, _center.z -_halfExtents.z);
	_maxPoint = Vector3(_center.x + _halfExtents.x, _center.y + _halfExtents.y, _center.z +_halfExtents.z);

	distance = this->_transform->GetPosition().x - other->_transform->GetPosition().x;
	distance = sqrtf(distance * distance);
	//calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	float combinedRadii = this->_halfExtents.x + other->GetExtents().x;
	//_debugOutputer->PrintDebugString("Combined Radii is: " + std::to_string(combinedRadii) + " Distance is: " + std::to_string(distance));

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
	//_debugOutputer->PrintDebugString("Combined Radii is: " + std::to_string(combinedRadii) + " Distance is: " + std::to_string(distance));

	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < combinedRadii)
	{
		collided = true;
	}
	else
	{
		return false; //immediately breaks out of the function and returns a failed collision check
	}

	distance = this->_transform->GetPosition().z - other->_transform->GetPosition().z;
	distance = sqrtf(distance * distance);
	//calculates the total (radius) size of the availble collission area (if you lay both bounding spheres next to each other)
	combinedRadii = this->_halfExtents.z + other->GetExtents().z;
	//_debugOutputer->PrintDebugString("Combined Radii is: " + std::to_string(combinedRadii) + " Distance is: " + std::to_string(distance));

	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < combinedRadii)
	{
		collided = true;
	}
	else
	{
		return false; //immediately breaks out of the function and returns a failed collision check
	}

	if (collided)
	{
		//_debugOutputer->PrintDebugString("COLLISION!");
	}

	return true;
}

bool BoxCollider::CollidesWith(PlaneCollider* other)
{
	return false;
}