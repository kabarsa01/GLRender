
#include "data/DataManager.h"
#include "core/Class.h"

DataManager* DataManager::Instance = nullptr;

DataManager::DataManager()
{

}

DataManager::~DataManager()
{

}

DataManager* DataManager::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = new DataManager();
	}

	return Instance;
}

void DataManager::ShutdownInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
	}
}

bool DataManager::AddResource(HashString InKey, shared_ptr<Resource> InValue)
{
	if (( InValue.get() != nullptr ) && ( ResourcesTable.find(InKey) == ResourcesTable.end() ))
	{
		ResourcesTable[InKey] = InValue;
		ResourcesMap[InValue->GetClass().GetName()][InKey] = InValue;
		return true;
	}

	return false;
}

bool DataManager::IsResourcePresent(HashString InKey)
{
	return ResourcesTable.find(InKey) != ResourcesTable.end();
}

shared_ptr<Resource> DataManager::GetResource(HashString InKey)
{
	return GetResource(InKey, ResourcesTable);
}

ResourcePtr DataManager::GetResource(HashString InKey, map<HashString, ResourcePtr>& InMap)
{
	map<HashString, ResourcePtr>::iterator It = InMap.find(InKey);
	if (It != InMap.end())
	{
		return It->second;
	}
	return nullptr;
}


