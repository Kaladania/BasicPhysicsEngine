#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include "Camera.h"
#include "Structures.h"
#include "OBJLoader.h"

#include <vector>

#include "GameObject.h"
#include "Timer.h"
#include "DebugOutputer.h"

#include "Transform.h"
#include "Renderer.h"

#define FIXED_DELTA_VALUE 1.0f/60.f //defines the amount of time each frame update should (on average) take


using namespace DirectX;

class DX11PhysicsFramework
{
private:

	int _WindowWidth = 1280;
	int _WindowHeight = 768;

	HWND _windowHandle;

	ID3D11DeviceContext* _immediateContext = nullptr;
	ID3D11Device* _device;
	IDXGIDevice* _dxgiDevice = nullptr;
	IDXGIFactory2* _dxgiFactory = nullptr;
	ID3D11RenderTargetView* _frameBufferView = nullptr;
	IDXGISwapChain1* _swapChain;
	D3D11_VIEWPORT _viewport;

	ID3D11VertexShader* _vertexShader;
	ID3D11InputLayout* _inputLayout;
	ID3D11PixelShader* _pixelShader;
	ID3D11Buffer* _constantBuffer;
	ID3D11Buffer* _cubeVertexBuffer;
	ID3D11Buffer* _cubeIndexBuffer;

	ID3D11Buffer* _planeVertexBuffer;
	ID3D11Buffer* _planeIndexBuffer;

	ID3D11DepthStencilView* _depthBufferView = nullptr;
	ID3D11Texture2D* _depthStencilBuffer = nullptr;

	ID3D11ShaderResourceView* _StoneTextureRV = nullptr;
	ID3D11ShaderResourceView* _GroundTextureRV = nullptr;
	ID3D11ShaderResourceView* _HerculesTextureRV = nullptr;

	ID3D11SamplerState* _samplerLinear = nullptr;

	Light basicLight;

	MeshData _objMeshData;
	std::vector<GameObject*> _gameObjects;

	Camera * _camera = nullptr;
	float _cameraOrbitRadius = 7.0f;
	float _cameraOrbitRadiusMin = 2.0f;
	float _cameraOrbitRadiusMax = 50.0f;
	float _cameraOrbitAngleXZ = -90.0f;
	float _cameraSpeed = 2.0f;

	ConstantBuffer _cbData;

	ID3D11DepthStencilState* _DSLessEqual;
	ID3D11RasterizerState* _RSCullNone;

	ID3D11RasterizerState* _CCWcullMode; //Counter Clockwise
	ID3D11RasterizerState* _CWcullMode; //Clockwise


	Timer* _timer = nullptr; //holds reference to framework's timer
	DebugOutputer* _debugOutputer = nullptr; //holds a reference to framework's debug outputter
	float _elapsedSeconds = 0; //holds the amount of seconds elapsed since last update

	float _currentMovementKeyPressDuration = 1; //holds how many frames the current movement key has been held down for
	char _lastMovementKeyPressed = '0'; //holds the last movement key to be pressed

	std::vector<int>ManualMovingObjects; //holds a list of all the objects currrently being manually moved.

	float a = 0;

private:
	HRESULT CreateWindowHandle(HINSTANCE hInstance, int nCmdShow);
	HRESULT CreateD3DDevice();
	HRESULT CreateSwapChainAndFrameBuffer();
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexIndexBuffers();
	HRESULT InitPipelineStates();
	HRESULT InitRunTimeData();

public:
	~DX11PhysicsFramework();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	bool HandleKeyboard(MSG msg);
	void GetMovementInput();
	void Update();
	void UpdatePhysics(float deltaTime = 0.0f);
	void ResolveCollisions();
	void Draw();
};

