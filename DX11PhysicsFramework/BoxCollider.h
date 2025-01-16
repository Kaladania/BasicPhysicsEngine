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
	BoxCollider(GameObject* parent);
	~BoxCollider();

	//gets the current collission radius of the collider
	void SetExtents(Vector3 extents = Vector3());
	Vector3 GetExtents() { return _halfExtents; }

	bool CollidesWith(Collider* other) override;
	bool CollidesWith(SphereCollider* other) override;
	bool CollidesWith(BoxCollider* other) override;

};

