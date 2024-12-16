#include "AutomatedMovement.h"

AutomatedMovement::AutomatedMovement()
{

}

AutomatedMovement::~AutomatedMovement()
{

}

/// <summary>
/// Updates the position of the connected transform
/// </summary>
/// <param name="deltaTime">time elapsed since last physics update</param>
void AutomatedMovement::Update(float deltaTime)
{
	Vector3 position = _transform->GetPosition(); //gets the current position of the transform
	position += _velocity * deltaTime; //updates position by a constant velocity (multiplied by deltaTime to find distance)
	_transform->SetPosition(position); //sets new transform position
}