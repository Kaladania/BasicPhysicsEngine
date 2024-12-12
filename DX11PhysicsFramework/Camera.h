#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "DebugOutputer.h"
#include "Vector3D.h"

using namespace DirectX;

class Camera
{
private:
	Vector3 _eye; 
	Vector3 _at;
	Vector3 _up;

	FLOAT _windowWidth;
	FLOAT _windowHeight;
	FLOAT _nearDepth;
	FLOAT _farDepth;

	XMFLOAT4X4 _view;
	XMFLOAT4X4 _projection;

	DebugOutputer* _debugOutputer = nullptr; //stores a reference to the object's debug outputer

public:
	Camera(Vector3 position, Vector3 at, Vector3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~Camera();

	void Update();

	XMFLOAT4X4 GetView() const { return _view; }
	XMFLOAT4X4 GetProjection() const { return _projection; }

	XMFLOAT4X4 GetViewProjection() const;

	Vector3 GetPosition() const { return _eye; }
	Vector3 GetLookAt() const { return _at; }
	Vector3 GetUp() const { return _up; }

	void SetPosition(Vector3 position) { _eye = position; }
	void SetLookAt(Vector3 lookAt) { _at = lookAt; }
	void SetUp(Vector3 up) { _up = up; }

	void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
};

