#pragma once

#include <memory>

#define THIS_PTR(Type) std::dynamic_pointer_cast<Type>(shared_from_this())

class ObjectBase : public std::enable_shared_from_this<ObjectBase>
{
public:
	ObjectBase();
	virtual ~ObjectBase();

	template <typename Derived>
	std::shared_ptr<Derived> derived_shared_from_this();

	// static object creation methods
	/*template <typename Type>
	static std::shared_ptr<Type> NewObject();*/
	template <typename Type, typename ...ArgTypes>
	static std::shared_ptr<Type> NewObject(ArgTypes ...Args);

	virtual void Initialize();
};

//-----------------------------------------------------------------------------------
// Templated definitions
//-----------------------------------------------------------------------------------

template <typename Derived>
std::shared_ptr<Derived> ObjectBase::derived_shared_from_this()
{
	return std::dynamic_pointer_cast<Derived>(shared_from_this());
}

//template<typename Type>
//inline std::shared_ptr<Type> ObjectBase::NewObject()
//{
//	std::shared_ptr<Type> NewObject = std::make_shared<Type>();
//	NewObject->Initialize();
//	return NewObject;
//}

template<typename Type, typename ...ArgTypes>
inline std::shared_ptr<Type> ObjectBase::NewObject(ArgTypes ...Args)
{
	std::shared_ptr<Type> NewObject = std::make_shared<Type>(Args...);
	NewObject->Initialize();
	return NewObject;
}




