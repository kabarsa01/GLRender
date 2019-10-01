#pragma once

#include <map>
#include <set>
#include <string>
#include <memory>

#include "core/ObjectBase.h"
#include "common/HashString.h"
#include "data/Resource.h"

using namespace std;

class DataManager
{
public:
	static DataManager* GetInstance();
	static void ShutdownInstance();

	bool AddResource(HashString InKey, shared_ptr<Resource> InValue);
	bool IsResourcePresent(HashString InKey);
	shared_ptr<Resource> GetResource(HashString InKey);
	template<class T>
	shared_ptr<T> GetResourceType(HashString InKey);
protected:
	map<HashString, ResourcePtr> ResourcesTable;
	map<HashString, map<HashString, ResourcePtr>> ResourcesMap;
private:
	static DataManager* Instance;

	DataManager();
	virtual ~DataManager();
};

//==================================================================================

template<class T>
shared_ptr<T> DataManager::GetResourceType(HashString InKey)
{
	return dynamic_pointer_cast<T>(GetResource(InKey));
}

