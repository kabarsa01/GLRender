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
	template <typename Type>
	static std::shared_ptr<Type> NewObject();
	template <typename Type, typename ParentType>
	static std::shared_ptr<Type> NewObject(std::shared_ptr<ObjectBase> InParent);

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

template<typename Type>
inline std::shared_ptr<Type> ObjectBase::NewObject()
{
	std::shared_ptr<Type> NewObject = std::make_shared<Type>();
	NewObject->Initialize();
	return NewObject;
}

template<typename Type, typename ParentType>
inline std::shared_ptr<Type> ObjectBase::NewObject(std::shared_ptr<ObjectBase> InParent)
{
	std::shared_ptr<Type> NewObject = std::make_shared<Type>(std::dynamic_pointer_cast<ParentType>(InParent));
	NewObject->Initialize();
	return NewObject;
}


