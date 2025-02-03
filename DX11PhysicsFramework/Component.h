#pragma once
#include "DebugOutputer.h"
#include "Quaternion.h"
//#include "GameObject.h"

class GameObject;
/// <summary>
/// Responsible for handling shared methods and attributes between all components
/// </summary>
class Component
{
protected:

	GameObject* _parent = nullptr; //stores a reference to the component's parent
	Vector3D* _vector3D = nullptr; //stores a reference to the component's vector3 manager
	DebugOutputer* _debugOutputer = nullptr; //stores a reference to the component's debug outputer
	bool _isActive = true; //states if the current componet is active

public:

	Component(GameObject* parent);
	~Component();

	//Sets/Gets the componet's activation status
	void SetIsActive(const bool status) { _isActive = status; }
	bool GetIsActive() { return _isActive; }


};

