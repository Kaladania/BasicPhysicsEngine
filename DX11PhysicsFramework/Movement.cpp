#include "Movement.h"

Movement::Movement(GameObject* parent) : Component(parent)
{
	_gravity = Vector3(0, -9.81f, 0); //sets gravity to be a downward force of 9.81
	DirectX::XMStoreFloat3x3(&_inertiaTensorMatrix, XMMatrixIdentity()); //defaults the inertia matrix to an identiy matrix
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

/// <summary>
/// Returns the object's inverse mass
/// </summary>
/// <returns>Inverse mass</returns>
float Movement::GetInverseMass()
{
	//function was created in order to catch attempts to find the inverse mass of objects with 0 mass
	//prevents calculations breaking in an attempt to divide by zero
	if (_mass == 0)
	{
		return 0;
	}

	return 1.0f / _mass;
}

Vector3 Movement::CalculateDisplacement(Vector3 displacement, float deltaTime)
{
	displacement = (_velocity * deltaTime) + ((_acceleration * (deltaTime * deltaTime)) * 0.5f);
	return displacement;
}

/// <summary>
/// Calculates the amount of drag applied to a floating object
/// </summary>
/// <returns>Current Drag Force</returns>
Vector3 Movement::CalculateDragForce()
{
	//gets the magnitude of the current velocity
	float velocityMagnitude = _vector3D->GetMagnitude(_velocity);
	
	//determines the scalar drag co-efficent
	float drag = DENSITY_OF_FLUID * ((velocityMagnitude * velocityMagnitude) * 0.5f) * _dragCoefficient * CROSS_SECTIONAL_AREA;

	//creates a base drag force that is the negated normal of the current velocity
	//multiplies base opposing force by the drag scalar to determine the intensity of drag
	Vector3 dragForce = (_vector3D->Normalize(_velocity) * -1) * drag;

	//returns the total drag force
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
	//_velocity = Vector3(0, 0, 0);
}

void Movement::CalculateImpulse(Movement* otherMovement, CollisionManifold manifold)
{
	//ApplyImpulse(Vector3(10, 0, 0));
	//determines the collision normal
	//the direction between the centers of the colliding objects
	//Vector3 collisionNormal = _vector3D->Normalize(_transform->GetPosition() - otherMovement->_transform->GetPosition());

	Vector3 collisionNormal = manifold.collisionNormal;

	//gets the relative velocity of the current object with respect to the incoming object
	Vector3 relativeVelocity = _velocity - otherMovement->GetVeclocity();

	//float collisionDotProduct = (collisionNormal.x * relativeVelocity.x) + (collisionNormal.y * relativeVelocity.y) + (collisionNormal.z * relativeVelocity.z);
	float collisionDotProduct = _vector3D->DotProduct(collisionNormal, relativeVelocity);
	//_debugOutputer->PrintDebugString("Collision Dot Product is" + std::to_string(collisionDotProduct));

	if (collisionDotProduct < 0.0f)
	{
		//velocity impulse
		float vj = -(1 + RESTITUTION_COEFFICIENT) * collisionDotProduct;

		//calculates the total impulse applied in the collision
		float J = vj / (GetInverseMass() + otherMovement->GetInverseMass());

		Vector3 impulseForce = collisionNormal * GetInverseMass() * J;
		
		Vector3 offset = collisionNormal * manifold.points[0].penetrationDepth * GetInverseMass();

		_transform->SetPosition(_transform->GetPosition() + offset);
;		//applies an impulse to the current object to propel it away from the other object
		ApplyImpulse(impulseForce);
		//_debugOutputer->PrintDebugString("Object Impulse is" + _vector3D->ToString(impulseForce));

		impulseForce = (collisionNormal * J * otherMovement->GetInverseMass()) * -1.0f;

		offset = (collisionNormal * manifold.points[0].penetrationDepth * otherMovement->GetInverseMass()) * -1;

		otherMovement->GetTransform()->SetPosition(otherMovement->GetTransform()->GetPosition() + offset);
		//applies the same impulse in the reverse direction move the other object away from the current object
		otherMovement->ApplyImpulse(impulseForce);
		//_debugOutputer->PrintDebugString("Other Object Impulse is" +  _vector3D->ToString(impulseForce));
	}

	//_debugOutputer->PrintDebugString("Velocity is now " + _vector3D->ToString(_velocity));
}

/// <summary>
/// Adds a specified amount of force to the velocity
/// </summary>
/// <param name="impulse">Amount of force to increase velocity by</param>
void Movement::ApplyImpulse(Vector3 impulse)
{
	_velocity += impulse;
}


/// <summary>
/// Override to add a relative force
/// </summary>
/// <param name="deltaTime"></param>
void Movement::AddRelativeForce(Vector3 force, Vector3 originPoint)
{
	//calculates the amount of force being applied - dependant on the angle of impact
	Vector3 relativePosition =  originPoint;
	_torque = _vector3D->CrossProduct(relativePosition, force);

	//particle body just adds the force, doesn't require torque calculation

	AddForce(_torque);
}

/// <summary>
/// Updates the values of the inertia matrix
/// </summary>
/// <param name="halfExtents">The half extents used to populate the matrix</param>
void Movement::SetInertiaMatrix(Vector3 halfExtents)
{
	float inertia = (1.0f / 12.0f) * _mass * ((halfExtents.y * halfExtents.y) + (halfExtents.z * halfExtents.z));

	_inertiaTensorMatrix._11 = inertia;
	//_debugOutputer->PrintDebugString("inertia is: " + std::to_string(inertia));

	inertia = (1.0f / 12.0f) * _mass * ((halfExtents.x * halfExtents.x) + (halfExtents.z * halfExtents.z));
	_inertiaTensorMatrix._22 = inertia;
	//_debugOutputer->PrintDebugString("inertia is: " + std::to_string(inertia));

	inertia = (1.0f / 12.0f) * _mass * ((halfExtents.x * halfExtents.x) + (halfExtents.y * halfExtents.y));
	_inertiaTensorMatrix._33 = inertia;
	//_debugOutputer->PrintDebugString("inertia is: " + std::to_string(inertia));
}

/// <summary>
/// Updates the values of the inertia matrix
/// </summary>
/// <param name="radius">The radius used to populate the matrix</param>
void Movement::SetInertiaMatrix(float radius)
{
	float inertia = (2.0f / 5.0f) * _mass * (pow(radius, 2));

	_inertiaTensorMatrix._11 = inertia;
	_inertiaTensorMatrix._22 = inertia;
	_inertiaTensorMatrix._33 = inertia;
}

void Movement::CalculateAngularMovement(float deltaTime)
{
	//ensures only objects with mass can rotate
	if (_mass > 0)
	{
		XMFLOAT3 angularAcceleration;
		XMVECTOR inverseInertia;

		//similar to friction, adds an inverse force that will aim to overtake and restrict the rotation of the object if not enough torque is applied
		//more than comparison limits opposing force to only being applied if torque increases
		if (_vector3D->GetMagnitude(_torque) > 0)
		{
			_torque -= _torque * 0.5f;
		}

		XMFLOAT3 convertedTorque;
		convertedTorque.x = _torque.x;
		convertedTorque.y = _torque.y;
		convertedTorque.z = _torque.z;

		//angularAcceleration; //holds the calculated angular acceleration
		Vector3 convertedAcceleration = Vector3(0, 0, 0); //holds a copy of the angular acceleration
		//inverseInertia; //holds the inverted inertia matrix

		//calculates the angular acceleration
		//inverse of inertia * torque
		DirectX::XMStoreFloat3(&angularAcceleration, XMVector3Transform(XMLoadFloat3(&convertedTorque), XMMatrixInverse(nullptr, DirectX::XMLoadFloat3x3(&_inertiaTensorMatrix))));

		//converts the acceleration from DirectX11 Vector3 to the custom made Vector3 in order to work with the Quaternion class calculations
		convertedAcceleration.x = angularAcceleration.x;
		convertedAcceleration.y = angularAcceleration.y;
		convertedAcceleration.z = angularAcceleration.z;

		//calculates the new angular velocity
		_angularVelocity += convertedAcceleration * deltaTime;

		//rotates the object based on its new velocity
		Quaternion orientation = _transform->GetOrientation();
		orientation += orientation * _angularVelocity * 0.5f * deltaTime;

		//checks to ensure the quaternion does not drift above/below 1qqq
		float magnitude = orientation.Magnitude();
		if (magnitude != 0)
		{
			//normalises quaterion
			orientation /= magnitude;
		}

		//_debugOutputer->PrintDebugString("Orientation: " + std::to_string(orientation.GetVector().x) + std::to_string(orientation.GetVector().y) + std::to_string(orientation.GetVector().z));
		_transform->SetOrientation(orientation);

		//dampens velocity
		_angularVelocity = _angularVelocity * powf(ANGULAR_DAMPING, deltaTime);
	}
}

/// <summary>
/// Updates the position of the connected transform
/// </summary>
/// <param name="deltaTime">time elapsed since last physics update</param>
void Movement::Update(float deltaTime)
{
	if (!_isStationary)
	{
		
		//calculates resistance force based on if object is suspended in air (not colliding) or moving along a surface (colliding)
		if (_isColliding)
		{
			//calculates the max value the friction scalar (intensity) should be to allow the object to move
			float maxFriction = _vector3D->GetMagnitude((_gravity * _mass) * STATIC_FRICTION_COEFFICIENT);

			//negates the netForce if the object does not have enough force to overcome friction (resets force applied to 0
			if (_vector3D->GetMagnitude(_netForce) < maxFriction)
			{
				_netForce = Vector3(0, 0, 0);
			}

			//calculates friction applied on a movement across a surface
			//applied even if static force is applied to ensure objects still decelerate if netForce drops below max while object is moving
			//e.g. if the propulsing force stops being applied
			_netForce -= CalulateFrictionForce();
		}
		else
		{
			//calculates drag applied ( air resistance )
			_netForce += CalculateDragForce();
		}

		//checks if the object is current simulating gravity
		if (_isSimulatingGravity)
		{
			_netForce += _gravity * _mass; //calculates the intensity of the gravitational force acting on the parent object
		}


		CalculateAngularMovement(deltaTime);

		//_debugOutputer->PrintDebugString("Net Force: " + _vector3D->ToString(_netForce));

		_acceleration += _netForce / _mass; //calculates current rate of acceleration


		Vector3 position = _transform->GetPosition(); //gets the current position of the transform

		//calculates current velocity
		//v = u + at
		_velocity += _acceleration * deltaTime;

		//calculates the distance moved during this frame and updates position
		position += _velocity * deltaTime; 

		if (position.y < -5.0f)
		{
			position.y = 10.0f;
		}

		////hard coded stop to prevent falling through platform
		////REMOVE AFTER COLLISSION IS IMPLIMENTED
		//if (_usesForcedFloor && position.y < 1)
		//{
		//	position.y = 1;
		//	_isColliding = true;
		//}
		//else
		//{
		//	_isColliding == false;
		//}

		_transform->SetPosition(position); //sets new transform position

		_debugOutputer->PrintDebugString("Position is: " + _vector3D->ToString(position));

		//resets force values to maintain intergrity of calculations
		_netForce = Vector3(0, 0, 0);
		_acceleration = Vector3(0, 0, 0);
		_oldVelocity = _velocity;
		//_velocity = Vector3(0, 0, 0);
	}
}
