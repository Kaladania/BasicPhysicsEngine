#pragma once
#include "Collider.h"
#include "BoxCollider.h"

class GameObject;
class BoxCollider;
class PlaneCollider;

class PlaneCollider : public Collider
{
private:

	Vector3 _normal = Vector3(); //holds the normal to the plane
	float _distanceFromNormal = 0.0f;


public:
	PlaneCollider(GameObject* parent, Transform* transform);
	~PlaneCollider();

	Vector3 GetPlaneNormal() const { return _normal; }
	void SetPlaneNormal(Vector3 normal) { _normal = _vector3D->Normalize(normal); }

	float GetDistanceFromNormal() const{ return _distanceFromNormal; }

	bool CollidesWith(Collider* other) override;
	bool CollidesWith(SphereCollider* other) override;
	bool CollidesWith(BoxCollider* other) override;
	bool CollidesWith(PlaneCollider* other) override;

};
