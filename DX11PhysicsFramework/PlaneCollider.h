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

	bool CollidesWith(Collider* other, CollisionManifold& manifold) override;
	bool CollidesWith(SphereCollider* other, CollisionManifold& manifold) override;
	bool CollidesWith(BoxCollider* other, CollisionManifold& manifold) override;
	bool CollidesWith(PlaneCollider* other, CollisionManifold& manifold) override;

};
