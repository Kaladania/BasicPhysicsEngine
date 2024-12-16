#include "Movement.h"

Movement::Movement(GameObject* parent) : Component(parent)
{
	
}

Movement::~Movement()
{
	_transform = nullptr;
}

/// <summary>
/// Translates an object in the requested direction
/// </summary>
/// <param name="transform">reference to object's transform</param>
/// <param name="direction">direction to translate the object</param>
void Movement::MoveTransform(Directions direction)
{
	Vector3 position = _transform->GetPosition(); //gets the objects current position

	Vector3 directionVector = Vector3(0, 0, 0.0f); //stores the new translation direction vector

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

	_velocity = directionVector;

	_needsToMove = true;

	////translates the object
	//position.x += directionVector.x;
	//position.y += directionVector.y;
	//position.z += directionVector.z;

	////updates the transform's position
	//_transform->SetPosition(position);
}

Vector3 Movement::CalculateDisplacement(Vector3 displacement, float deltaTime)
{
	displacement = (_velocity * deltaTime) + ((_acceleration * (deltaTime * deltaTime)) * 0.5f);
	return displacement;
}

/// <summary>
/// Updates the position of the connected transform
/// </summary>
/// <param name="deltaTime">time elapsed since last physics update</param>
void Movement::Update(float deltaTime)
{
	if (_needsToMove)
	{
		Vector3 position = _transform->GetPosition(); //gets the current position of the transform

		Vector3 displacement = (_velocity * deltaTime) + ((_acceleration * (deltaTime * deltaTime)) * 0.5f);

		position += displacement; //updates position by a constant velocity (multiplied by deltaTime to find distance)

		_transform->SetPosition(position); //sets new transform position
	}

	if (!_isAutomatic)
	{
		_needsToMove = false;
	}
}