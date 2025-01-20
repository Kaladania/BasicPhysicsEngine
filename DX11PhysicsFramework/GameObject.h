#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

//components
#include "Transform.h"
#include "Renderer.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "DebugOutputer.h"
#include "Vector3D.h"

#include "PhysicsBody.h"
#include "RigidBody.h"
#include "set"

using namespace DirectX;
//using namespace std;

enum Components
{
	TransformComponent,
	RendererComponent,
	RigidbodyComponent,
	ParticleComponent
};


class GameObject
{

private:
	GameObject* _parent = nullptr;  //stores a reference to the object's parent

	//components
	Transform* _transform = nullptr; //stores a reference to the object's transform information
	Renderer* _renderer = nullptr; //stores a reference to the object's renderer
	
	PhysicsBody* _physicsBody = nullptr; //stores a refernce to the object's physics body
	PhysicsType _physicsType = NOPHYSICS; //stores a reference to the type of physics body paired to the object

	//holds object type
	std::string _type;
	XMFLOAT4X4 _world;

	DebugOutputer* _debugOutputer = nullptr; //stores a reference to the object's debug outputer

	std::set<Components> _componentList; //stores a list of current componenets
	


public:
	GameObject(std::string type);
	~GameObject();

	std::string GetType() const { return _type; }

	void SetParent(GameObject * parent) { _parent = parent; }

	//Gets components
	Transform* GetTransform() const { return _transform; }
	Renderer* GetRenderer() const { return _renderer; }
	PhysicsBody* GetPhysicsBody() const { return _physicsBody; }
	PhysicsType GetPhysicsType() const { return _physicsType; }

	
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void AddComponent(Components componentType);
	bool ContainsComponent(Components componentType);



	void Update(float dt);
	void Draw(ID3D11DeviceContext * pImmediateContext);

};

