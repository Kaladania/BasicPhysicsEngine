#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

//components
#include "Transform.h"
#include "Renderer.h"
#include "Movement.h"
#include "DebugOutputer.h"
#include "Vector3D.h"

using namespace DirectX;
//using namespace std;

enum Components
{
	TransformComponent,
	RendererComponent,
	MovementComponent
};


class GameObject
{

private:
	GameObject* _parent = nullptr;  //stores a reference to the object's parent

	//components
	Transform* _transform = nullptr; //stores a reference to the object's transform information
	Renderer* _renderer = nullptr; //stores a reference to the object's renderer
	Movement* _movement = nullptr; //stores a reference to the object's movement component
	
	//holds object type
	std::string _type;
	XMFLOAT4X4 _world;

	DebugOutputer* _debugOutputer = nullptr; //stores a reference to the object's debug outputer
	


public:
	GameObject(std::string type);
	~GameObject();

	std::string GetType() const { return _type; }

	void SetParent(GameObject * parent) { _parent = parent; }

	//Gets components
	Transform* GetTransform() const { return _transform; }
	Renderer* GetRenderer() const { return _renderer; }
	Movement* GetMovement() const { return _movement; }


	
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void AddComponent(Components componentType);



	void Update(float dt);
	void Move(Vector3 direction);
	void Draw(ID3D11DeviceContext * pImmediateContext);

};

