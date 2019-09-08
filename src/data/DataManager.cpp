
#include "data/DataManager.h"

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

bool DataManager::AddResource(string InKey, shared_ptr<Resource> InValue)
{
	if (( InValue.get() != nullptr ) && ( Resources.find(InKey) != Resources.end() ))
	{
		Resources[InKey] = InValue;
		return true;
	}

	return false;
}

bool DataManager::IsResourcePresent(string InKey)
{
	return Resources.find(InKey) != Resources.end();
}

shared_ptr<Resource> DataManager::GetResource(string InKey)
{
	map<string, shared_ptr<Resource>>::iterator It = Resources.find(InKey);
	if (It != Resources.end())
	{
		return It->second;
	}
	return make_shared<Resource>( InKey );
}

