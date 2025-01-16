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


public:
	PlaneCollider(GameObject* parent);
	~PlaneCollider();

	bool CollidesWith(Collider* other) override;
	bool CollidesWith(SphereCollider* other) override;
	bool CollidesWith(BoxCollider* other) override;
	bool CollidesWith(PlaneCollider* other) override;

};
