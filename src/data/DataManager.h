#pragma once

#include <map>
#include <string>
#include <memory>

#include "core/ObjectBase.h"
#include "data/Resource.h"

using namespace std;

class DataManager
{
public:
	static DataManager* GetInstance();
	static void ShutdownInstance();

	bool AddResource(string InKey, shared_ptr<Resource> InValue);
	bool IsResourcePresent(string InKey);
	shared_ptr<Resource> GetResource(string InKey);
	template<class T>
	shared_ptr<T> GetResourceType(string InKey);
protected:
	map<string, shared_ptr<Resource>> Resources;
private:
	static DataManager* Instance;

	DataManager();
	virtual ~DataManager();
};

//==================================================================================

template<class T>
shared_ptr<T> DataManager::GetResourceType(string InKey)
{
	return dynamic_pointer_cast<T>(GetResource(InKey));
}

