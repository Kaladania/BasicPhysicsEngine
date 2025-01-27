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

Vector3 Movement::CalculateDragForce()
{
	//drag = 1/2(fluid density * speed^2 * COD * cross-section area

	float velocityMagnitude = _vector3D->GetMagnitude(_velocity);
	
	float drag = 0.5f * DENSITY_OF_FLUID * (velocityMagnitude * velocityMagnitude) * DRAG_COEFFICIENT * CROSS_SECTIONAL_AREA;
	_debugOutputer->PrintDebugString(std::to_string(drag) + " " + std::to_string(velocityMagnitude));

	//creates a base drag force that is the negated normal of the current velocity
	Vector3 dragForce = (_vector3D->Normalize(_velocity) * -1) * drag;



	return dragForce;
}

/// <summary>
/// Calculates the amount of friction opposing the movement
/// </summary>
/// <returns></returns>
Vector3 Movement::CalulateFrictionForce()
{
	//determines the force currently propelling the object
	Vector3 normalForce = _velocity * _mass;
	
	//returns the amount of friction generated by the movement
	return normalForce * FRICTION_COEFFICIENT;
}

void Movement::CalculateCollisionResolutionForce(const float otherCOR)
{
	_netForce = ((_velocity * -2.0f)) + (((_velocity * -2)) * _COR);
	_debugOutputer->PrintDebugString("Velocity is: " + _vector3D->ToString(_velocity));
	_velocity = Vector3(0, 0, 0);
}

void Movement::CalculateImpulse(Vector3 otherPosition, Movement* otherMovement)
{
	//gets the direction between objects
	Vector3 collisionNormal = _vector3D->Normalize(_transform->GetPosition() - otherPosition);

	float restitution = 0; //hard coded for testing

	Vector3 relativeVelocity = _velocity - otherMovement->GetVeclocity();

	Vector3 vj = relativeVelocity * _vector3D->GetMagnitude((collisionNormal * (-(1 + restitution))));

	Vector3 J = vj * ((-1 / _mass) + (-1 / otherMovement->GetMass()));

	ApplyImpulse(_vector3D->CrossProduct(J * (-1 / _mass), collisionNormal));
	ApplyImpulse((_vector3D->CrossProduct(J * (-1 / _mass), collisionNormal)) * -1);
}

void Movement::ApplyImpulse(Vector3 impulse)
{
	_velocity += impulse;
}

/// <summary>
/// Updates the position of the connected transform
/// </summary>
/// <param name="deltaTime">time elapsed since last physics update</param>
void Movement::Update(float deltaTime)
{
	if (!_isStationary)
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

		_debugOutputer->PrintDebugString(_vector3D->ToString(_dragForce));

		_debugOutputer->PrintDebugString("Net Force is: " + _vector3D->ToString(_netForce));
		_netForce -= CalulateFrictionForce();
		_debugOutputer->PrintDebugString("Net Force after Friction is: " + _vector3D->ToString(_netForce));
		//_netForce += CalculateDragForce();

		_acceleration += _netForce / _mass; //calculates current rate of acceleration

		//_debugOutputer->PrintDebugString(_vector3D->ToString(CalulateFrictionForce()));

		Vector3 position = _transform->GetPosition(); //gets the current position of the transform

		_velocity += _acceleration * _mass; //calculates current velocity

		position += _velocity * deltaTime; //calculates the distance moved during this frame and updates position

		//hard coded stop to prevent falling through platform
		//REMOVE AFTER COLLISSION IS IMPLIMENTED
		if (_usesForcedFloor && position.y < 1)
		{
			position.y = 1;
		}

		_transform->SetPosition(position); //sets new transform position

		//resets force values to maintain intergrity of calculations
		_netForce = Vector3(0, 0, 0);
		_acceleration = Vector3(0, 0, 0);
		//_velocity = Vector3(0, 0, 0);
	}
}