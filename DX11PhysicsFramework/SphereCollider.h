#pragma once
#include "Collider.h"
//#include "BoxCollider.h"

class GameObject;

class SphereCollider : public Collider
{
private:
	float _radius = 0.0f;
	

public:
	SphereCollider(GameObject* parent, Transform* transform);
	~SphereCollider();

	//gets the current collission radius of the collider
	void SetCollissionRadius(float collissionRadius = 0.0f) { _radius = collissionRadius; }
	float GetCollissionRadius() const { return _radius; } 

	bool CollidesWith(Collider* other, CollisionManifold& manifold) override;
	bool CollidesWith(SphereCollider* other, CollisionManifold& manifold) override;
	bool CollidesWith(BoxCollider* other, CollisionManifold& manifold) override;
	bool CollidesWith(PlaneCollider* other, CollisionManifold& manifold) override;

};

