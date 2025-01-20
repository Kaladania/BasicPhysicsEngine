#include "DX11PhysicsFramework.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

bool DX11PhysicsFramework::HandleKeyboard(MSG msg)
{
	Vector3 cameraPosition = _camera->GetPosition();

	switch (msg.wParam)
	{
	case VK_UP:
		_cameraOrbitRadius = max(_cameraOrbitRadiusMin, _cameraOrbitRadius - (_cameraSpeed * 0.2f));
		return true;
		break;

	case VK_DOWN:
		_cameraOrbitRadius = min(_cameraOrbitRadiusMax, _cameraOrbitRadius + (_cameraSpeed * 0.2f));
		return true;
		break;

	case VK_RIGHT:
		_cameraOrbitAngleXZ -= _cameraSpeed;
		return true;
		break;

	case VK_LEFT:
		_cameraOrbitAngleXZ += _cameraSpeed;
		return true;
		break;
	}

	return false;
}

HRESULT DX11PhysicsFramework::Initialise(HINSTANCE hInstance, int nShowCmd)
{
	HRESULT hr = S_OK;

	hr = CreateWindowHandle(hInstance, nShowCmd);
	if (FAILED(hr)) return E_FAIL;

	hr = CreateD3DDevice();
	if (FAILED(hr)) return E_FAIL;

	hr = CreateSwapChainAndFrameBuffer();
	if (FAILED(hr)) return E_FAIL;

	hr = InitShadersAndInputLayout();
	if (FAILED(hr)) return E_FAIL;

	hr = InitVertexIndexBuffers();
	if (FAILED(hr)) return E_FAIL;

	hr = InitPipelineStates();
	if (FAILED(hr)) return E_FAIL;

	hr = InitRunTimeData();
	if (FAILED(hr)) return E_FAIL;

	return hr;
}

HRESULT DX11PhysicsFramework::CreateWindowHandle(HINSTANCE hInstance, int nCmdShow)
{
	const wchar_t* windowName = L"DX11Framework";

	WNDCLASSW wndClass;
	wndClass.style = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = 0;
	wndClass.hIcon = 0;
	wndClass.hCursor = 0;
	wndClass.hbrBackground = 0;
	wndClass.lpszMenuName = 0;
	wndClass.lpszClassName = windowName;

	RegisterClassW(&wndClass);

	_windowHandle = CreateWindowExW(0, windowName, windowName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, _WindowWidth, _WindowHeight, nullptr, nullptr, hInstance, nullptr);

	return S_OK;
}

HRESULT DX11PhysicsFramework::CreateD3DDevice()
{
	HRESULT hr = S_OK;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
	};

	ID3D11Device* baseDevice;
	ID3D11DeviceContext* baseDeviceContext;

	DWORD createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT | createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &baseDevice, nullptr, &baseDeviceContext);
	if (FAILED(hr)) return hr;

	///////////////////////////////////////////////////////////////////////////////////////////////

	hr = baseDevice->QueryInterface(__uuidof(ID3D11Device), reinterpret_cast<void**>(&_device));
	hr = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext), reinterpret_cast<void**>(&_immediateContext));

	baseDevice->Release();
	baseDeviceContext->Release();

	///////////////////////////////////////////////////////////////////////////////////////////////

	hr = _device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&_dxgiDevice));
	if (FAILED(hr)) return hr;

	IDXGIAdapter* dxgiAdapter;
	hr = _dxgiDevice->GetAdapter(&dxgiAdapter);
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&_dxgiFactory));
	dxgiAdapter->Release();

	return S_OK;
}

HRESULT DX11PhysicsFramework::CreateSwapChainAndFrameBuffer()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	swapChainDesc.Width = 0; // Defer to WindowWidth
	swapChainDesc.Height = 0; // Defer to WindowHeight
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //FLIP* modes don't support sRGB backbuffer
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = 0;

	hr = _dxgiFactory->CreateSwapChainForHwnd(_device, _windowHandle, &swapChainDesc, nullptr, nullptr, &_swapChain);
	if (FAILED(hr)) return hr;

	///////////////////////////////////////////////////////////////////////////////////////////////

	ID3D11Texture2D* frameBuffer = nullptr;

	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&frameBuffer));
	if (FAILED(hr)) return hr;

	D3D11_RENDER_TARGET_VIEW_DESC framebufferDesc = {};
	framebufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //sRGB render target enables hardware gamma correction
	framebufferDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hr = _device->CreateRenderTargetView(frameBuffer, &framebufferDesc, &_frameBufferView);

	frameBuffer->Release();

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	frameBuffer->GetDesc(&depthBufferDesc); // copy from framebuffer properties

	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	_device->CreateTexture2D(&depthBufferDesc, nullptr, &_depthStencilBuffer);
	_device->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthBufferView);

	return hr;
}

HRESULT DX11PhysicsFramework::InitShadersAndInputLayout()
{

	HRESULT hr = S_OK;
	ID3DBlob* errorBlob;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* vsBlob;

    // Compile the vertex shader
    hr = D3DCompileFromFile(L"SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_main", "vs_5_0", dwShaderFlags, 0, &vsBlob, &errorBlob);
	if (FAILED(hr))
	{
		MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
		errorBlob->Release();
		return hr;
	}

	// Create the vertex shader
	hr = _device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &_vertexShader);

	if (FAILED(hr))
	{	
		vsBlob->Release();
        return hr;
	}

	// Compile the pixel shader
	ID3DBlob* psBlob;
	hr = D3DCompileFromFile(L"SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0", dwShaderFlags, 0, &psBlob, &errorBlob);
	if (FAILED(hr))
	{
		MessageBoxA(_windowHandle, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
		errorBlob->Release();
		return hr;
	}

	// Create the pixel shader
	hr = _device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &_pixelShader);

    if (FAILED(hr))
        return hr;
	
    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

    // Create the input layout
	hr = _device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &_inputLayout);
	
	vsBlob->Release();
	psBlob->Release();
	errorBlob->Release();

	return hr;
}

HRESULT DX11PhysicsFramework::InitVertexIndexBuffers()
{
	HRESULT hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0, 1.0f, 0), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0, 1.0f, 0), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0, 1.0f, 0), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0, 1.0f, 0), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0, -1.0f, 0), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0, -1.0f, 0), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0, -1.0f, 0), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0, -1.0f, 0), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT2(1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(1.0f, 0.0f) },
    };

    D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;

    hr = _device->CreateBuffer(&bd, &InitData, &_cubeVertexBuffer);

    if (FAILED(hr))
        return hr;

	// Create index buffer
	WORD indices[] =
	{
		3, 1, 0,
		2, 1, 3,

		6, 4, 5,
		7, 4, 6,

		11, 9, 8,
		10, 9, 11,

		14, 12, 13,
		15, 12, 14,

		19, 17, 16,
		18, 17, 19,

		22, 20, 21,
		23, 20, 22
	};

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	hr = _device->CreateBuffer(&bd, &InitData, &_cubeIndexBuffer);

	if (FAILED(hr))
		return hr;

	// Create vertex buffer
	SimpleVertex planeVertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 5.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(5.0f, 5.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(5.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	};

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = planeVertices;

	hr = _device->CreateBuffer(&bd, &InitData, &_planeVertexBuffer);

	if (FAILED(hr))
		return hr;

	// Create plane index buffer
	WORD planeIndices[] =
	{
		0, 3, 1,
		3, 2, 1,
	};

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = planeIndices;
	hr = _device->CreateBuffer(&bd, &InitData, &_planeIndexBuffer);

	if (FAILED(hr))
		return hr;

	return S_OK;
}


HRESULT DX11PhysicsFramework::InitPipelineStates()
{
	HRESULT hr = S_OK;

	_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_immediateContext->IASetInputLayout(_inputLayout);

	// Rasterizer
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	hr = _device->CreateRasterizerState(&cmdesc, &_RSCullNone);

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = true;
	hr = _device->CreateRasterizerState(&cmdesc, &_CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = _device->CreateRasterizerState(&cmdesc, &_CWcullMode);

	_immediateContext->RSSetState(_CWcullMode);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	_device->CreateDepthStencilState(&dssDesc, &_DSLessEqual);

	_immediateContext->OMSetDepthStencilState(_DSLessEqual, 0);

	D3D11_SAMPLER_DESC bilinearSamplerdesc = {};
	bilinearSamplerdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	bilinearSamplerdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	bilinearSamplerdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	bilinearSamplerdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	bilinearSamplerdesc.MaxLOD = INT_MAX;
	bilinearSamplerdesc.MinLOD = 0;

	hr = _device->CreateSamplerState(&bilinearSamplerdesc, &_samplerLinear);
	if (FAILED(hr)) return hr;

    return S_OK;
}

HRESULT DX11PhysicsFramework::InitRunTimeData()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	_viewport = { 0.0f, 0.0f, (float)_WindowWidth, (float)_WindowHeight, 0.0f, 1.0f };
	_immediateContext->RSSetViewports(1, &_viewport);

	hr = _device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffer);
	if (FAILED(hr)) { return hr; }

	hr = CreateDDSTextureFromFile(_device, L"Resources\\Textures\\stone.dds", nullptr, &_StoneTextureRV);
	hr = CreateDDSTextureFromFile(_device, L"Resources\\Textures\\floor.dds", nullptr, &_GroundTextureRV);
	if (FAILED(hr)) { return hr; }

	// Setup Camera
	Vector3 eye = Vector3(0.0f, 2.0f, -1.0f);
	Vector3 at = Vector3(0.0f, 2.0f, 0.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

	_camera = new Camera(eye, at, up, (float)_WindowWidth, (float)_WindowHeight, 0.01f, 200.0f);

	// Setup the scene's light
	basicLight.AmbientLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	basicLight.DiffuseLight = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	basicLight.SpecularLight = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	basicLight.SpecularPower = 10.0f;
	basicLight.LightVecW = XMFLOAT3(0.0f, 0.5f, -1.0f);

	Geometry sphereGeometry;
	_objMeshData = OBJLoader::Load("Resources\\OBJ\\sphere.obj", _device);
	sphereGeometry.indexBuffer = _objMeshData.IndexBuffer;
	sphereGeometry.numberOfIndices = _objMeshData.IndexCount;
	sphereGeometry.vertexBuffer = _objMeshData.VertexBuffer;
	sphereGeometry.vertexBufferOffset = _objMeshData.VBOffset;
	sphereGeometry.vertexBufferStride = _objMeshData.VBStride;

	Geometry cubeGeometry;
	cubeGeometry.indexBuffer = _cubeIndexBuffer;
	cubeGeometry.vertexBuffer = _cubeVertexBuffer;
	cubeGeometry.numberOfIndices = 36;
	cubeGeometry.vertexBufferOffset = 0;
	cubeGeometry.vertexBufferStride = sizeof(SimpleVertex);

	Geometry planeGeometry;
	planeGeometry.indexBuffer = _planeIndexBuffer;
	planeGeometry.vertexBuffer = _planeVertexBuffer;
	planeGeometry.numberOfIndices = 6;
	planeGeometry.vertexBufferOffset = 0;
	planeGeometry.vertexBufferStride = sizeof(SimpleVertex);

	Material shinyMaterial;
	shinyMaterial.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	shinyMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	shinyMaterial.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	Material noSpecMaterial;
	noSpecMaterial.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	GameObject* gameObject = new GameObject("Floor", 0);

	//adds and populates transformation component
	gameObject->AddComponent(TransformComponent);
	Transform* objectTransform = gameObject->GetTransform();
	objectTransform->SetPosition(0.0f, 0.0f, 0.0f);
	objectTransform->SetScale(15.0f, 15.0f, 15.0f);
	objectTransform->SetRotation(XMConvertToRadians(90.0f), 0.0f, 0.0f);

	//adds and populates render information
	gameObject->AddComponent(RendererComponent);
	Renderer* objectRenderer = gameObject->GetRenderer();
	objectRenderer->SetGeometry(planeGeometry);
	objectRenderer->SetMaterial(noSpecMaterial);
	objectRenderer->SetTextureRV(_GroundTextureRV);

	//adds and populates render information
	gameObject->AddComponent(RigidbodyComponent);
	PhysicsBody* objectBody = gameObject->GetPhysicsBody();
	objectBody->SetCollider(BOX_COLLISSION_COMPONET);
	Collider* objectCollider = objectBody->GetCollider();
	objectBody->GetMovement()->SetIsStationary(true); //sets the floor as stationary
	static_cast<BoxCollider*>(objectCollider)->SetExtents(Vector3(30.0f, 0.0f, 30.0f));
	

	_gameObjects.push_back(gameObject);

	Movement* objectMovement = nullptr;

	for (auto i = 0; i < 4; i++)
	{
		gameObject = new GameObject("Cube " + i, i + 1);

		//adds and populates transformation component
		gameObject->AddComponent(TransformComponent);
		objectTransform = gameObject->GetTransform();
		objectTransform->SetScale(1.0f, 1.0f, 1.0f);
		objectTransform->SetPosition(-2.0f + (i * 2.5f), 5.0f, 10.0f);

		//adds and populates render information
		gameObject->AddComponent(RendererComponent);
		objectRenderer = gameObject->GetRenderer();
		objectRenderer->SetGeometry(cubeGeometry);
		objectRenderer->SetMaterial(shinyMaterial);
		objectRenderer->SetTextureRV(_StoneTextureRV);

		//adds and populates movement information

		//objectMovement = gameObject->GetPhysicsBody()->GetMovement();
		
		//objectMovement->SetTransform(objectTransform); //ties movement component to the object's transformation

		

		//adds and populates render information
		gameObject->AddComponent(RigidbodyComponent);

		PhysicsBody* objectBody = gameObject->GetPhysicsBody(); //adds a physics body
		objectBody->SetCollider(BOX_COLLISSION_COMPONET); //sets the type of collider the body should use

		Collider* objectCollider = objectBody->GetCollider(); //gets a reference to the collider
		static_cast<BoxCollider*>(objectCollider)->SetExtents(Vector3(2.0f, 2.0f, 2.0f)); //sets the dimensions of the collider

		objectCollider->SetIsActive(false); //disables collision on the object

		objectMovement = gameObject->GetPhysicsBody()->GetMovement();
		objectMovement->SetIsSimulatingGravity(true); //states second object is going to be influenced by gravity


		if (i <= 1)
		{
			objectMovement->SetMovementSpeed(3.0f);
		}

		//sets the first square to collide
		if (i == 0)
		{
			objectCollider->SetIsActive(true);
		}
		
		_gameObjects.push_back(gameObject);
	}

	//_gameObjects.back()->GetMovement()->SetVelocity(Vector3(0, 1, 0)); //sets the last cube to constantly ascend updwards
	//_gameObjects.back()->GetMovement()->SetAcceleration(Vector3(0, 3.0f, 0));

	gameObject = new GameObject("Sphere", 5);

	//adds and populates transformation component
	gameObject->AddComponent(TransformComponent);
	objectTransform = gameObject->GetTransform();
	objectTransform->SetScale(1.0f, 1.0f, 1.0f);
	objectTransform->SetPosition(-5.0f, 2.0f, 10.0f);

	//adds and populates render information
	gameObject->AddComponent(RendererComponent);
	objectRenderer = gameObject->GetRenderer();
	objectRenderer->SetGeometry(sphereGeometry);
	objectRenderer->SetMaterial(shinyMaterial);
	objectRenderer->SetTextureRV(_StoneTextureRV);

	_gameObjects.push_back(gameObject);

	_timer = new Timer();

	return S_OK;
}

DX11PhysicsFramework::~DX11PhysicsFramework()
{
	delete _camera;
	for each (GameObject * go in _gameObjects)
	{
		delete go;
	}

	if (_immediateContext)_immediateContext->Release();

	if (_frameBufferView)_frameBufferView->Release();
	if (_depthBufferView)_depthBufferView->Release();
	if (_depthStencilBuffer)_depthStencilBuffer->Release();
	if (_swapChain)_swapChain->Release();
	if (_CWcullMode)_CWcullMode->Release();
	if (_CCWcullMode)_CCWcullMode->Release();
	if (_vertexShader)_vertexShader->Release();
	if (_inputLayout)_inputLayout->Release();
	if (_pixelShader)_pixelShader->Release();
	if (_constantBuffer)_constantBuffer->Release();

	if (_cubeVertexBuffer)_cubeVertexBuffer->Release();
	if (_cubeIndexBuffer)_cubeIndexBuffer->Release();
	if (_planeVertexBuffer)_planeVertexBuffer->Release();
	if (_planeIndexBuffer)_planeIndexBuffer->Release();
	if (_objMeshData.IndexBuffer) _objMeshData.IndexBuffer->Release();
	if (_objMeshData.VertexBuffer)_objMeshData.VertexBuffer->Release();

	if (_DSLessEqual) _DSLessEqual->Release();
	if (_RSCullNone) _RSCullNone->Release();

	if (_samplerLinear)_samplerLinear->Release();
	if (_StoneTextureRV)_StoneTextureRV->Release();
	if (_GroundTextureRV)_GroundTextureRV->Release();
	if (_HerculesTextureRV)_HerculesTextureRV->Release();

	if (_dxgiDevice)_dxgiDevice->Release();
	if (_dxgiFactory)_dxgiFactory->Release();
	if (_device)_device->Release();

	delete _timer;
	_timer = nullptr;

	delete _debugOutputer;
	_debugOutputer = nullptr;
}

void DX11PhysicsFramework::Update()
{
	//delays physics updated to specified FPS (FIXED_DELTA_VALUE)
	if (_elapsedSeconds >= FIXED_DELTA_VALUE)
	{
		UpdatePhysics(FIXED_DELTA_VALUE); //runs a physics update

		//minuses value instead of resetting to 0 to account for lost time
		_elapsedSeconds -= FIXED_DELTA_VALUE;

		//records new update frame
		_timer->Tick();
	}
	else
	{
		//continues to count if not yet time to update physics
		_elapsedSeconds += _timer->GetDeltaTime();
	}
	

	// Update camera
	float angleAroundZ = XMConvertToRadians(_cameraOrbitAngleXZ);

	float x = _cameraOrbitRadius * cos(angleAroundZ);
	float z = _cameraOrbitRadius * sin(angleAroundZ);

	Vector3 cameraPos = _camera->GetPosition();
	cameraPos.x = x;
	cameraPos.z = z;

	_camera->SetPosition(cameraPos);
	

	// Update objects
	for (auto gameObject : _gameObjects)
	{
		gameObject->Update(_elapsedSeconds);
	}
	
}

void DX11PhysicsFramework::UpdatePhysics(float deltaTime)
{
	char _currentMovementKeyPressed = '0'; //stores the current movement key that was pressed
	
	// Move gameobjects
	// Add force applies an accelerational force that is intensified by the time the object has been excellerating
	if (GetAsyncKeyState('1'))
	{
		_gameObjects[1]->GetPhysicsBody()->GetMovement()->AddForce(Vector3(0, 0, -1.0f) * _currentMovementKeyPressDuration);
		_currentMovementKeyPressed = '1';
	}
	if (GetAsyncKeyState('2'))
	{
		_gameObjects[1]->GetPhysicsBody()->GetMovement()->AddForce(Vector3(0, 0, 1.0f) * _currentMovementKeyPressDuration);
		_currentMovementKeyPressed = '2';
	}
	if (GetAsyncKeyState('3'))
	{
		_gameObjects[2]->GetPhysicsBody()->GetMovement()->AddForce(Vector3(0, 0, -1.0f) * _currentMovementKeyPressDuration);
		_currentMovementKeyPressed = '3';
	}
	if (GetAsyncKeyState('4'))
	{
		_gameObjects[2]->GetPhysicsBody()->GetMovement()->AddForce(Vector3(0, 0, 1.0f) * _currentMovementKeyPressDuration);
		_currentMovementKeyPressed = '4';
	}

	//checks if the object has been told to switch movement directions (or stop moving)
	if (_currentMovementKeyPressed == _lastMovementKeyPressed)
	{
		//if not, increase acceleration intensity
		_currentMovementKeyPressDuration += 0.005f;
		
	}
	else
	{
		//else, reset acceleration
		_currentMovementKeyPressDuration = 1;
	}

	//records the key currently being pressed
	_lastMovementKeyPressed = _currentMovementKeyPressed;

	_camera->Update();


	// Update objects
	for (auto gameObject : _gameObjects)
	{
		//if the current game object has a physics componet and is actively looking for collissions
		if (gameObject->ContainsComponent(RigidbodyComponent) && gameObject->GetPhysicsBody()->GetCollider()->GetIsActive())
		{
			
			for (auto object : _gameObjects)
			{
				//ensures objects do not attempt to collision check against themselves
				//ensures object has a collission component and is also actively looking for collissions
				if (object != gameObject && object->ContainsComponent(RigidbodyComponent) && object->GetPhysicsBody()->GetCollider()->GetIsActive())
				{
					//gets a reference to all colliders
					Collider* collider = gameObject->GetPhysicsBody()->GetCollider();
					Collider* otherCollider = object->GetPhysicsBody()->GetCollider();

					//checks if a collision occures
					if (collider->CheckForCollission(otherCollider))
					{
						_debugOutputer->PrintDebugString("COLLISSION!");
						gameObject->GetPhysicsBody()->GetMovement()->CalculateImpulse(object->GetTransform()->GetPosition(), object->GetPhysicsBody()->GetMovement());

						//checks to see if the colliding object has a movement component (and so has a custom COR)
						if (object->ContainsComponent(RigidbodyComponent))
						{
							//gameObject->GetPhysicsBody()->GetMovement()->CalculateCollisionResolutionForce(object->GetPhysicsBody()->GetMovement()->GetCOR());
							
							object->GetPhysicsBody()->GetMovement()->CalculateImpulse(object->GetTransform()->GetPosition(), object->GetPhysicsBody()->GetMovement());
						}
						//else
						//{
						//	//assumes the colliding object is completely stationary and/or a wall
						//	gameObject->GetPhysicsBody()->GetMovement()->ApplyImpulse(Vector3(-1, 0, 0));
						//	//gameObject->GetPhysicsBody()->GetMovement()->CalculateCollisionResolutionForce(0);
						//}

					}
				}
			}

			gameObject->GetPhysicsBody()->UpdatePhysics(deltaTime);
		}

		

	}

}

void DX11PhysicsFramework::Draw()
{
    //
    // Clear buffers
    //
	float ClearColor[4] = { 0.25f, 0.25f, 0.75f, 1.0f }; // red,green,blue,alpha
	_immediateContext->OMSetRenderTargets(1, &_frameBufferView, _depthBufferView);
    _immediateContext->ClearRenderTargetView(_frameBufferView, ClearColor);
	_immediateContext->ClearDepthStencilView(_depthBufferView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //
    // Setup buffers and render scene
    //
	_immediateContext->VSSetShader(_vertexShader, nullptr, 0);
	_immediateContext->PSSetShader(_pixelShader, nullptr, 0);

	_immediateContext->VSSetConstantBuffers(0, 1, &_constantBuffer);
	_immediateContext->PSSetConstantBuffers(0, 1, &_constantBuffer);
	_immediateContext->PSSetSamplers(0, 1, &_samplerLinear);

	XMFLOAT4X4 tempView = _camera->GetView();
	XMFLOAT4X4 tempProjection = _camera->GetProjection();

	XMMATRIX view = XMLoadFloat4x4(&tempView);
	XMMATRIX projection = XMLoadFloat4x4(&tempProjection);

	_cbData.View = XMMatrixTranspose(view);
	_cbData.Projection = XMMatrixTranspose(projection);
	
	_cbData.light = basicLight;

	Vector3 vector = _camera->GetPosition();
	_cbData.EyePosW = XMFLOAT3(vector.x, vector.y, vector.z);

	Renderer* objectRenderer = nullptr; //holds a reference to the current object's renderer

	// Render all scene objects
	for (auto gameObject : _gameObjects)
	{
		objectRenderer = gameObject->GetRenderer(); //gets current renderer

		// Get render material
		Material material = objectRenderer->GetMaterial();

		// Copy material to shader
		_cbData.surface.AmbientMtrl = material.ambient;
		_cbData.surface.DiffuseMtrl = material.diffuse;
		_cbData.surface.SpecularMtrl = material.specular;

		// Set world matrix
		_cbData.World = XMMatrixTranspose(gameObject->GetWorldMatrix());

		// Set texture
		if (objectRenderer->HasTexture())
		{
			_immediateContext->PSSetShaderResources(0, 1, objectRenderer->GetTextureRV());
			_cbData.HasTexture = 1.0f;
		}
		else
		{
			_cbData.HasTexture = 0.0f;
		}

		//Write constant buffer data onto GPU
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		_immediateContext->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
		_immediateContext->Unmap(_constantBuffer, 0);

		// Draw object
		gameObject->Draw(_immediateContext);
	}

    //
    // Present our back buffer to our front buffer
    //
    _swapChain->Present(0, 0);
}