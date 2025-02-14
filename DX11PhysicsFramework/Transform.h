#pragma once
#include <directxmath.h>
#include <string>
#include "Component.h"

using namespace DirectX;
/// <summary>
/// Responsible for storing and operating on transformation information
/// </summary>
class Transform : public Component
{
private:

	//object transformation vectors
	Vector3 _position; //holds the current position of the object
	Vector3 _spawnPoint = Vector3(0, 0, 0); //holds the original spawn point of the object

	//for improved readability, publicly reffered to as a 'rotation' when expecting Euler Angles from the user
	//class converts and uses Quaternions - refered to as 'orientation' - for improved performance
	Quaternion _orientation;
	Vector3 _scale;

public:

	Transform(GameObject* parent = nullptr);
	~Transform();

	// Setters and Getters for position/rotation/scale
	void SetPosition(Vector3 position) { _position = position; }
	void SetPosition(float x, float y, float z) { _position.x = x; _position.y = y; _position.z = z; }

	void SetSpawnPoint(Vector3 position) { _spawnPoint = position; }
	Vector3 GetSpawnPoint() const { return _spawnPoint; }


	Vector3 GetPosition() const { return _position; }

	void SetScale(Vector3 scale) { _scale = scale; }
	void SetScale(float x, float y, float z) { _scale.x = x; _scale.y = y; _scale.z = z; }

	Vector3 GetScale() const { return _scale; }

	void SetRotation(Vector3 rotation) { _orientation = MakeQFromEulerAngles(rotation.x, rotation.y, rotation.z); }
	void SetRotation(float x, float y, float z) { _orientation = MakeQFromEulerAngles(x, y, z); }

	void SetOrientation(Quaternion orientation) { _orientation = orientation; }

	Vector3 GetRotation() const { return MakeEulerAnglesFromQ(_orientation); }
	Quaternion GetOrientation() const { return _orientation; }
};

