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

	bool _isPlane = false; //states if the current box collider is attached to a plane


public:
	BoxCollider(GameObject* parent, Transform* transform);
	~BoxCollider();

	//gets the current collission radius of the collider
	void SetExtents(Vector3 extents = Vector3());
	Vector3 GetExtents() { return _halfExtents; }

	void SetIsPlane(bool isPlane) { _isPlane = isPlane; }
	bool GetIsPlane() const { return _isPlane; }

	bool CollidesWith(Collider* other) override;
	bool CollidesWith(SphereCollider* other) override;
	bool CollidesWith(BoxCollider* other) override;
	bool CollidesWith(PlaneCollider* other) override;

	float Clamp(float value = 0.0f, float min = 0.0f, float max = 0.0f);
	float GetSquareAxisDistance(float sphereAxisValue = 0.0f, float AABBAxisValueMin = 0.0f, float AABBAxisValueMax = 0.0f);
	float CalculateDistanceBetweenAxis(float aMin = 0.0f, float aMax = 0.0f, float bMin = 0.0f, float bMax = 0.0f);

};

