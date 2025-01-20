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

/// <summary>
/// Responsible for calculating and applying collissions
/// </summary>
class Collider abstract : public Component
{

protected:

	Transform* _transform = nullptr; //stores reference of the transform the component is tied to

public:

	Collider(GameObject* parent = nullptr, Transform* transform = nullptr);
	~Collider();

	void SetTransform(Transform* transform = nullptr) { _transform = transform; }
	Transform* GetTransform(){return _transform;}

	//collission check functions + overloads
	virtual bool CollidesWith(Collider* other) = 0;
	virtual bool CollidesWith(SphereCollider* other) = 0;
	virtual bool CollidesWith(BoxCollider* other) = 0;
	virtual bool CollidesWith(PlaneCollider* other) = 0;

	bool CheckForCollission(Collider* other);
	
};

