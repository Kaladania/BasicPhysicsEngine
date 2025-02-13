#include "PlaneCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"



PlaneCollider::PlaneCollider(GameObject* parent, Transform* transform) : Collider(parent, transform)
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
bool PlaneCollider::CollidesWith(Collider* other)
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
bool PlaneCollider::CollidesWith(SphereCollider* other)
{
	//Vector3 path = this->_transform->GetPosition() - other->GetTransform()->GetPosition();
	//float distance = _vector3D->GetMagnitude(path);

	float distance = fabs(_vector3D->DotProduct(other->GetTransform()->GetPosition(), _normal)); //gets the distance between the sphere center and plane
	distance -= other->GetCollissionRadius(); //subtracts the radius to find the total distance from THE OUTSIDE OF THE SPHERE

	if (distance <= 0.0f)
	{
		//Vector3 path = this->_transform->GetPosition() - other->GetTransform()->GetPosition();
		//distance = _vector3D->GetMagnitude(path);

		_collisionManifold.collisionNormal = _normal; //stores direction of collision
		_collisionManifold.contactPointCount = 1; //stores amount of points involved in collission
		_collisionManifold.points[0].Position = _transform->GetPosition() + (_collisionManifold.collisionNormal * other->GetCollissionRadius()); //stores the position of the point of collision
		_collisionManifold.points[0].penetrationDepth = fabs(distance); //stores the amount of overlap involved in the collision

		return true;
	}



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
bool PlaneCollider::CollidesWith(BoxCollider* other)
{
	//Vector3 objectDirection = other->GetTransform()->GetPosition();

	float distance = fabs(_vector3D->DotProduct(other->GetTransform()->GetPosition(), _normal)); //gets the distance between the sphere center and plane
	distance -= other->GetExtents().y; //subtracts the radius to find the total distance from THE OUTSIDE OF THE SPHERE

	if (distance <= 0.0f)
	{
		//Vector3 path = this->_transform->GetPosition() - other->GetTransform()->GetPosition();
		//distance = _vector3D->GetMagnitude(path);

		_collisionManifold.collisionNormal = _normal; //stores direction of collision
		_collisionManifold.contactPointCount = 1; //stores amount of points involved in collission
		_collisionManifold.points[0].Position = _transform->GetPosition() + (_collisionManifold.collisionNormal * other->GetExtents().y); //stores the position of the point of collision
		_collisionManifold.points[0].penetrationDepth = fabs(distance); //stores the amount of overlap involved in the collision

		return true;
	}

	return false;
}

/// <summary>
/// Checks collission against another sphere collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool PlaneCollider::CollidesWith(PlaneCollider* other)
{
	return false;
}