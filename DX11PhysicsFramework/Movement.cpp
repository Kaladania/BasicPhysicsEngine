#include "Movement.h"

Movement::Movement()
{
	_debugOutputer = new DebugOutputer(); //instantiates a debug outputter
	_vector3D = new Vector3D(); //instantiates a 3D vector manager
}

Movement::~Movement()
{
	delete _debugOutputer;
	delete _vector3D;

	_transform = nullptr;
	_debugOutputer = nullptr;
	_vector3D = nullptr;
}

/// <summary>
/// Translates an object in the requested direction
/// </summary>
/// <param name="transform">reference to object's transform</param>
/// <param name="direction">direction to translate the object</param>
void Movement::MoveTransform(Directions direction)
{
	XMFLOAT3 position = _transform->GetPosition(); //gets the objects current position
	XMFLOAT3 directionVector = XMFLOAT3(0, 0, 0.0f); //stores the new translation direction vector

	//customises the direction vector based on the requested direction
	switch (direction)
	{
	case Left:
		break;
	case Right:
		break;
	case Up:
		break;
	case Down:
		break;
	case Forwards:

		directionVector.z = -_movementSpeed;
		break;
	case Backwards:

		directionVector.z = _movementSpeed;
		break;
	default:
		break;
	}

	//translates the object
	position.x += directionVector.x;
	position.y += directionVector.y;
	position.z += directionVector.z;

	//updates the transform's position
	_transform->SetPosition(position);

	Vector3 vector;
	vector.x = position.x;
	vector.y = position.y;
	vector.z = position.z;

1	_debugOutputer->PrintDebugString(_vector3D->ToString(vector));
}