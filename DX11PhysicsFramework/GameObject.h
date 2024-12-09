#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

//components
#include "Transform.h"

using namespace DirectX;
//using namespace std;

struct Geometry
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int numberOfIndices;

	UINT vertexBufferStride;
	UINT vertexBufferOffset;
};

struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
};

enum Components
{
	TransformComponent
};


class GameObject
{

private:
	GameObject* _parent = nullptr;  //stores a reference to the object's parent

	//components
	Transform* _transform = nullptr; //stores a reference to the object's transform information

	
	//holds object type
	std::string _type;
	XMFLOAT4X4 _world;

	//object info
	Geometry _geometry; //holds geometry info for rendering
	Material _material; //holds material info for lighting

	ID3D11ShaderResourceView* _textureRV = nullptr;

public:
	GameObject(std::string type, Geometry geometry, Material material);
	~GameObject();

	std::string GetType() const { return _type; }

	void SetParent(GameObject * parent) { _parent = parent; }

	//Gets components
	Transform* GetTransform() const { return _transform; }


	// Gets Rendering information
	Geometry GetGeometryData() const { return _geometry; }
	Material GetMaterial() const { return _material; }
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }

	void AddComponent(Components componentType);


	void SetTextureRV(ID3D11ShaderResourceView * textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView* const* GetTextureRV() { return &_textureRV; }
	bool HasTexture() const { return _textureRV ? true : false; }

	void Update(float dt);
	void Move(XMFLOAT3 direction);
	void Draw(ID3D11DeviceContext * pImmediateContext);

};

