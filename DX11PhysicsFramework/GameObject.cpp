#include "GameObject.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="type">object type</param>
/// <param name="geometry"></param>
/// <param name="material"></param>
GameObject::GameObject(std::string type)
{
	_parent = nullptr;
	_type = type;

	_debugOutputer = new DebugOutputer(); //instantiates a debug outputter
	//_vector3D = new Vector3D(); //instantiates a 3D vector manager

}

/// <summary>
/// Destructor
/// </summary>
GameObject::~GameObject()
{
	_parent = nullptr;

	delete _transform;
	delete _renderer;
	delete _movement;
	delete _debugOutputer;
	//delete _vector3D;

	_transform = nullptr;
	_renderer = nullptr;
	_movement = nullptr;
	_debugOutputer = nullptr;
	//_vector3D = nullptr;
	
}

/// <summary>
/// Updates object matrices and transformations
/// </summary>
/// <param name="deltaTime">time elapsed since last update</param>
void GameObject::Update(float deltaTime)
{
	// creates transformation matrices
	XMMATRIX scale = XMMatrixScaling(_transform->GetScale().x, _transform->GetScale().y, _transform->GetScale().z);
	XMMATRIX rotation = XMMatrixRotationX(_transform->GetRotation().x) * XMMatrixRotationY(_transform->GetRotation().y) * XMMatrixRotationZ(_transform->GetRotation().z);
	XMMATRIX translation = XMMatrixTranslation(_transform->GetPosition().x, _transform->GetPosition().y, _transform->GetPosition().z);

	//Calculates and updates object world matrix
	XMStoreFloat4x4(&_world, scale * rotation * translation);

	//checks to see if the object needs to move relative to the parent
	if (_parent != nullptr)
	{
		//if yes, calculate new position and update
		XMStoreFloat4x4(&_world, this->GetWorldMatrix() * _parent->GetWorldMatrix());
	}
}

void GameObject::UpdatePhysics(float deltaTime)
{
	//updates a movement component if it has been added and is active
	if (_movement != nullptr && _movement->GetIsActive())
	{
		_movement->Update(deltaTime);
	}

	
}

/// <summary>
/// Updates object's position
/// </summary>
/// <param name="direction">direction to translate position</param>
void GameObject::Move(Vector3 direction)
{
	Vector3 position = _transform->GetPosition();

	position.x += direction.x;
	position.y += direction.y;
	position.z += direction.z;

	_transform->SetPosition(position);
}

/// <summary>
/// Renders Game objects. RUNS AFTER MAIN FRAMEWORK DRAW SETUP
/// </summary>
/// <param name="pImmediateContext">Copy of the immediate context to draw the objects to</param>
void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{

	_renderer->Draw(pImmediateContext);
}

/// <summary>
/// Adds the requested component to the object
/// </summary>
/// <param name="componentType">component type to add</param>
void GameObject::AddComponent(Components componentType)
{
	switch (componentType)
	{
	case TransformComponent:

		_transform = new Transform;

		break;

	case RendererComponent:

		_renderer = new Renderer;

		break;

	case MovementComponent:

		_movement = new Movement;
		break;

	case SphereCollissionComponent:

		_collider = new SphereCollider(this);

	case BoxCollissionComponent:

		_collider = new BoxCollider(this);

	default:
		break;
	}
}