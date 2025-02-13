#include "SphereCollider.h"
#include "BoxCollider.h"
#include "PlaneCollider.h"

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

	_collisionManifold = CollisionManifold(); //clears the manifold each check


	//gets the distance between the centers of the colliding objects
	Vector3 path = this->_transform->GetPosition() - other->_transform->GetPosition();
	float distance = _vector3D->GetMagnitude(path) ;

	//gets the combined radius of both spheres
	float sumOfRadii = other->GetCollissionRadius() + _radius;

	//if the distance between centers is less than the combined radii, a collision has occured
	if (distance < sumOfRadii)
	{
		_collisionManifold.collisionNormal = _vector3D->Normalize(path); //stores direction of collision
		_collisionManifold.contactPointCount = 1; //stores amount of points involved in collission
		_collisionManifold.points[0].Position = _transform->GetPosition() + (_collisionManifold.collisionNormal * _radius); //stores the position of the point of collision
		_collisionManifold.points[0].penetrationDepth = fabsf(distance - sumOfRadii); //stores the amount of overlap involved in the collision

		return true;
	}

	
	return false;
}

/// <summary>
/// Checks collission against another sphere collider
/// </summary>
/// <param name="other">the other collider being collided with</param>
/// <returns>bool stating if collission occured</returns>
bool SphereCollider::CollidesWith(BoxCollider* other)
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

	float distance = fabs(clampedIntersection.x - center.x);

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

	distance = fabs(clampedIntersection.y - center.y);

	//if the distance is less than the combined radii, then the other object is within the collission area and is touching this object
	if (distance < _radius)
	{
		collided = true;
	}
	else
	{
		return false; //immediately breaks out of the function and returns a failed collision check
	}

	distance = fabs(clampedIntersection.z - center.z);

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

	//getting to this point means all checks have succeed and a collision has occured
	//thus collision manifold can be updated

	_collisionManifold = CollisionManifold();

	_collisionManifold.collisionNormal = _vector3D->Normalize(this->_transform->GetPosition() - other->GetTransform()->GetPosition()); //stores direction of collision
	_collisionManifold.contactPointCount = 1; //stores amount of points involved in collission
	_collisionManifold.points[0].Position = _transform->GetPosition() + (_collisionManifold.collisionNormal * _radius); //stores the position of the point of collision

	Vector3 otherPosition = other->GetTransform()->GetPosition();
	Vector3 position = _transform->GetPosition();

	float xOverlap = otherPosition.x - position.x;
	float yOverlap = otherPosition.y - position.y;
	float zOverlap = otherPosition.z - position.z;

	_collisionManifold.points[0].penetrationDepth = fabsf(_vector3D->GetMagnitude(this->_transform->GetPosition() - otherPosition) - (other->GetExtents().x + _radius)); //stores the smallest axis penetration as the penetration depth

	return true;
}

bool SphereCollider::CollidesWith(PlaneCollider* other)
{
	float distance = fabs(_vector3D->DotProduct(this->_transform->GetPosition(), other->GetPlaneNormal())); //gets the distance between the sphere center and plane
	distance -= _radius; //subtracts the radius to find the total distance from THE OUTSIDE OF THE SPHERE

	if (distance <= 0.0f)
	{
		//Vector3 path = this->_transform->GetPosition() - other->GetTransform()->GetPosition();
		//distance = _vector3D->GetMagnitude(path);

		_collisionManifold.collisionNormal = other->GetPlaneNormal(); //stores direction of collision
		_collisionManifold.contactPointCount = 1; //stores amount of points involved in collission
		_collisionManifold.points[0].Position = _transform->GetPosition() + (_collisionManifold.collisionNormal * _radius); //stores the position of the point of collision
		_collisionManifold.points[0].penetrationDepth = fabs(distance); //stores the amount of overlap involved in the collision

		return true;
	}

	return false;
}