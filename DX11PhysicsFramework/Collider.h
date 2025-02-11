#pragma once
#include <directxmath.h>
#include <string>
#include "Transform.h"
#include "Component.h"

class GameObject;
class SphereCollider;
class BoxCollider;
class PlaneCollider;

enum CollissionLayer
{
	WORLD,
	OBJECT,
	PLAYER
};

enum ColliderType
{
	NONE,
	SPHERE_COLLISSION_COMPONENT,
	BOX_COLLISSION_COMPONET
};

struct ContactPoint
{
	Vector3 Position;
	float penetrationDepth;
};

struct CollisionManifold
{
	int contactPointCount = 0;
	ContactPoint points[4];
	Vector3 collisionNormal;
};

/// <summary>
/// Responsible for calculating and applying collissions
/// </summary>
class Collider abstract : public Component
{

protected:

	Transform* _transform = nullptr; //stores reference of the transform the component is tied to
	ContactPoint _contactPoint; //holds the point of collision
	CollisionManifold _collisionManifold; //holds the collider's collision manifold

public:

	Collider(GameObject* parent = nullptr, Transform* transform = nullptr);
	~Collider();

	void SetTransform(Transform* transform = nullptr) { _transform = transform; }
	Transform* GetTransform(){return _transform;}

	CollisionManifold GetManifold() const { return _collisionManifold; }

	//collission check functions + overloads
	virtual bool CollidesWith(Collider* other, CollisionManifold& manifold) = 0;
	virtual bool CollidesWith(SphereCollider* other, CollisionManifold& manifold) = 0;
	virtual bool CollidesWith(BoxCollider* other, CollisionManifold& manifold) = 0;
	virtual bool CollidesWith(PlaneCollider* other, CollisionManifold& manifold) = 0;

	bool CheckForCollission(Collider* other, CollisionManifold& manifold);
	
};

