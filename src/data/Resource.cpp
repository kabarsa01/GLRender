
#include "data/Resource.h"
#include "data/DataManager.h"

Resource::Resource(std::string InId)
	: ObjectBase()
	, Id{ InId }
{
}

Resource::Resource()
	: ObjectBase{}
{

}

Resource::~Resource()
{

}

void Resource::Initialize()
{
	ObjectBase::Initialize();
	DataManager::GetInstance()->AddResource(Id, get_shared_from_this<Resource>());
}

std::string Resource::GetResourceId()
{
	return Id;
}

