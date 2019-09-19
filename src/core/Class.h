#pragma once

#include <string>
#include <memory>
#include <typeinfo>
#include <map>

#include "common/HashString.h"

class Class
{
public:
	template<class T>
	static const Class& Get();

	const HashString& GetName() const;

	bool operator==(const Class& Other) const;
private:
	static std::map<size_t, std::shared_ptr<Class>> Classes;
	HashString Name;

	Class();
	Class(const std::string& InName);
	Class(const HashString& InName);
	Class(const Class& InClass);
	virtual ~Class();

	Class& operator=(const Class& Other);
};

//==========================================================================================
//==========================================================================================

template<class T>
inline const Class & Class::Get()
{
	HashString HashStr{ typeid( T ).name() };

	if (Classes.find(HashStr.GetHash()) == Classes.end())
	{
		Classes.insert( std::pair<size_t, std::shared_ptr<Class>> ( HashStr.GetHash(), std::make_shared<Class>(HashStr) ) );
	}

	return * Classes.at(HashStr.GetHash());
}




