#pragma once

#include <string>
#include <memory>
#include <map>

#include "common/HashString.h"

template<typename T>
std::string GetTypeName() { return std::string{}; }

#define GET_TYPE_NAME(Type) \
template<> \
std::string GetTypeName<Type>() \
{ \
	return #Type; \
}

class Class
{
public:
	template<class T>
	static const Class& Get();// const HashString& InName);

	const HashString& GetName() const;
private:
	static std::map<size_t, std::shared_ptr<Class>> Classes;
	HashString Name;

	Class();
	Class(const std::string& InName);
	Class(const HashString& InName);
	Class(const Class& InName);
	virtual ~Class();

	Class& operator=(const Class& other);
	Class& operator=(Class&& other);
	Class& operator=(Class arg) noexcept;
};

//==========================================================================================
//==========================================================================================

template<class T>
inline const Class & Class::Get()
{
	HashString HashStr{ T::GetClassName() };

	if (Classes.find(HashStr.GetHash()) == Classes.end())
	{
		Classes.insert( std::pair<size_t, std::shared_ptr<Class>> { HashStr.GetHash(), std::make_shared<Class>(HashStr) } );
	}

	return * Classes.at(HashStr.GetHash());
}

#define DECLARE_CLASS(Type) \
friend class Class;


