#include "Movement.h"

Movement::Movement(GameObject* parent) : Component(parent)
{
	_gravity = Vector3(0, -9.81f, 0); //sets gravity to be a downward force of 9.81
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
}

Vector3 Movement::CalculateDisplacement(Vector3 displacement, float deltaTime)
{
	displacement = (_velocity * deltaTime) + ((_acceleration * (deltaTime * deltaTime)) * 0.5f);
	return displacement;
}

float Movement::CalculateDragForce()
{
	float velocityMagnitude = _vector3D->GetMagnitude(_velocity);
	
	float drag = 0.5 * DENSITY_OF_FLUID * (velocityMagnitude * velocityMagnitude) * DRAG_COEFFICIENT * CROSS_SECTIONAL_AREA;

	return drag;
}

Vector3 Movement::CalulateFrictionForce()
{
	Vector3 tangentialForce = (_gravity * _mass) * float(sin(0));
	Vector3 normalForce = (_gravity * _mass) * float(cos(0));
	Vector3 frictionForce = normalForce * float(FRICTION_COEFFICIENT); 

	/*if (tangentialForce > frictionForce)
	{
		return 
	}*/

	return _acceleration * float(FRICTION_COEFFICIENT);
}

void Movement::CalculateCollisionResolutionForce(const float otherCOR)
{
	_netForce = ((_netForce * -1)) * _COR;
}

/// <summary>
/// Updates the position of the connected transform
/// </summary>
/// <param name="deltaTime">time elapsed since last physics update</param>
void Movement::Update(float deltaTime)
{
	//checks if the object is current simulating gravity
	if (_isSimulatingGravity)
	{
		_netForce += _gravity * _mass; //calculates the intensity of the gravitational force acting on the parent object
	}

	//float dragScalar = CalculateDragForce();
	/*Vector3 norVelocity = _vector3D->Normalize(_velocity);
	_dragForce = norVelocity * dragScalar;
	_netForce += _dragForce;*/

	//_debugOutputer->PrintDebugString(_vector3D->ToString(_dragForce));
	

	_acceleration += _netForce / _mass; //calculates current rate of acceleration

	//_debugOutputer->PrintDebugString(_vector3D->ToString(CalulateFrictionForce()));

	Vector3 position = _transform->GetPosition(); //gets the current position of the transform

	_velocity = _acceleration * _mass; //calculates current velocity

	position += _velocity * deltaTime; //calculates the distance moved during this frame and updates position

	//hard coded stop to prevent falling through platform
	//REMOVE AFTER COLLISSION IS IMPLIMENTED
	/*if (position.y < 1)
	{
		position.y = 1;
	}*/

	_transform->SetPosition(position); //sets new transform position

	//resets force values to maintain intergrity of calculations
	_netForce = Vector3(0, 0, 0);
	_acceleration = Vector3(0, 0, 0);
}