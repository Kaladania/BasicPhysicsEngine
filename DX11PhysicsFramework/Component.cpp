#include "Component.h"

Component::Component(GameObject* parent)
{
	_parent = parent;
	_debugOutputer = new DebugOutputer(); //instantiates a debug outputter
	_vector3D = new Vector3D(); //instantiates a 3D vector manager
}

Component::~Component()
{
	delete _debugOutputer;
	delete _vector3D;

	_parent = nullptr;
	_debugOutputer = nullptr;
	_vector3D = nullptr;
}