#pragma once
#include <directxmath.h>
#include <d3d11_1.h>
#include <string>

using namespace DirectX;

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


/// <summary>
/// Responsible for storing and rendering object mesh information
/// </summary>
class Renderer
{
private:

	//object info
	Geometry _geometry; //holds geometry info for rendering
	Material _material; //holds material info for lighting

	ID3D11ShaderResourceView* _textureRV = nullptr;

public:

	Renderer();
	~Renderer();


	// Sets Rendering information
	void SetGeometry(Geometry geometry) { _geometry = geometry; }
	void SetMaterial(Material material) { _material = material; }

	// Gets Rendering information
	Geometry GetGeometryData() const { return _geometry; }
	Material GetMaterial() const { return _material; }


	void SetTextureRV(ID3D11ShaderResourceView* textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView* const* GetTextureRV() { return &_textureRV; }
	bool HasTexture() const { return _textureRV ? true : false; }

	void Draw(ID3D11DeviceContext* pImmediateContext);
};

