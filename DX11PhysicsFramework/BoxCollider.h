#pragma once
#include "Collider.h"

class GameObject;

class BoxCollider : public Collider
{
private:
	
	Vector3 _minPoint = Vector3(); //position of bottom left corner
	Vector3 _maxPoint = Vector3(); //position of top right corner
	Vector3 _center = Vector3(); //position of object center
	Vector3 _halfExtents = Vector3(); //position of the length of the object along each axis


public:
	BoxCollider(GameObject* parent, Transform* transform);
	~BoxCollider();

	//gets the current collission radius of the collider
	void SetExtents(Vector3 extents = Vector3());
	Vector3 GetExtents() { return _halfExtents; }

	bool CollidesWith(Collider* other, CollisionManifold& manifold) override;
	bool CollidesWith(SphereCollider* other, CollisionManifold& manifold) override;
	bool CollidesWith(BoxCollider* other, CollisionManifold& manifold) override;
	bool CollidesWith(PlaneCollider* other, CollisionManifold& manifold) override;

	float Clamp(float value = 0.0f, float min = 0.0f, float max = 0.0f);
	float GetSquareAxisDistance(float sphereAxisValue = 0.0f, float AABBAxisValueMin = 0.0f, float AABBAxisValueMax = 0.0f);
	float CalculateDistanceBetweenAxis(float aMin = 0.0f, float aMax = 0.0f, float bMin = 0.0f, float bMax = 0.0f);

};

