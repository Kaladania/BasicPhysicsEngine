#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

//components
#include "Transform.h"
#include "Renderer.h"

using namespace DirectX;
//using namespace std;

enum Components
{
	TransformComponent,
	RendererComponent
};


class GameObject
{

private:
	GameObject* _parent = nullptr;  //stores a reference to the object's parent

	//components
	Transform* _transform = nullptr; //stores a reference to the object's transform information
	Renderer* _renderer = nullptr; //stores a reference to the object's renderer
	
	//holds object type
	std::string _type;
	XMFLOAT4X4 _world;


public:
	GameObject(std::string type);
	~GameObject();

	std::string GetType() const { return _type; }

	void SetParent(GameObject * parent) { _parent = parent; }

	//Gets components
	Transform* GetTransform() const { return _transform; }
	Renderer* GetRenderer() const { return _renderer; }


	
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void AddComponent(Components componentType);



	void Update(float dt);
	void Move(XMFLOAT3 direction);
	void Draw(ID3D11DeviceContext * pImmediateContext);

};

