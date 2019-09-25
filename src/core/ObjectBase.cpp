#include "core/ObjectBase.h"
#include "core/Class.h"

ObjectBase::ObjectBase()
{
}

ObjectBase::~ObjectBase()
{
}

const Class & ObjectBase::GetClass()
{
	return * InstanceClass;
}

void ObjectBase::Initialize()
{
	InstanceClass = Class::GetClass(this);
}

void ObjectBase::Destroy()
{
}

