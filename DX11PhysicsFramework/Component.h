#pragma once
#include "DebugOutputer.h"
#include "Vector3D.h"
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

public:

	Component(GameObject* parent);
	~Component();


};

