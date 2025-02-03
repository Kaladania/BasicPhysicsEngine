#include "GameObject.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="type">object type</param>
/// <param name="geometry"></param>
/// <param name="material"></param>
GameObject::GameObject(std::string type, float id)
{
	_parent = nullptr;
	_type = type;
	_objectID = id;

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
	delete _physicsBody;

	delete _debugOutputer;
	//delete _vector3D;

	_transform = nullptr;
	_renderer = nullptr;
	_physicsBody = nullptr;
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

	//loads the quaternion from the transform and converts it into a rotation matrix
	Vector3 vector = _transform->GetOrientation().GetVector();
	XMVECTOR orientation = XMVectorSet(vector.x, vector.y, vector.z, _transform->GetOrientation().GetScalar());
	XMMATRIX rotation = XMMatrixRotationQuaternion(orientation);
	
	//XMMATRIX rotation = XMMatrixRotationX(_transform->GetRotation().x) * XMMatrixRotationY(_transform->GetRotation().y) * XMMatrixRotationZ(_transform->GetRotation().z);
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



/// <summary>
/// Renders Game objects. RUNS AFTER MAIN FRAMEWORK DRAW SETUP
/// </summary>
/// <param name="pImmediateContext">Copy of the immediate context to draw the objects to</param>
void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{

	_renderer->Draw(pImmediateContext);
}

/// <summary>
/// Adds the requested component to the object and the component list
/// </summary>
/// <param name="componentType">component type to add</param>
void GameObject::AddComponent(Components componentType)
{
	switch (componentType)
	{
	case TransformComponent:

		_transform = new Transform(this);
		_componentList.insert(TransformComponent);

		break;

	case RendererComponent:

		_renderer = new Renderer();
		_componentList.insert(RendererComponent);
		break;

	case RigidbodyComponent:

		_physicsBody = new RigidBody(this, _transform);
		_componentList.insert(RigidbodyComponent);
		break;

	/*case ParticleComponent:

		_physicsBody = new RigidBody;
		_componentList.insert(RigidbodyComponent);
		break;*/

	/*case MovementComponent:

		_movement = new Movement;
		_componentList.insert(MovementComponent);
		break;

	case SphereCollissionComponent:

		_collider = new SphereCollider(this);
		_componentList.insert(SphereCollissionComponent);

	case BoxCollissionComponent:

		_collider = new BoxCollider(this);
		_componentList.insert(BoxCollissionComponent);*/

	default:
		break;
	}
}

/// <summary>
/// States if the game object holds a requested component
/// </summary>
/// <param name="componentType">The component to check</param>
/// <returns>states if the component has been found on the object</returns>
bool GameObject::ContainsComponent(Components componentType)
{
	std::set<Components>::iterator it;
	it = _componentList.find(componentType);

	//checks to see if the returned iterator is an actual index
	if (it != _componentList.end())
	{
		return true;
	}

	return false;
}