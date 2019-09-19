#include "core/Class.h"

const HashString & Class::GetName() const
{
	return Name;
}

bool Class::operator==(const Class & Other) const
{
	return this->Name == Other.Name;
}

Class::Class()
	: Name( HashString::NONE() )
{
}

Class::Class(const std::string & InName)
	: Name( InName )
{
}

Class::Class(const HashString & InName)
	: Name( InName )
{
}

Class::Class(const Class & InClass)
	: Name( InClass.Name )
{
}

Class::~Class()
{
}

Class & Class::operator=(const Class & other)
{
	return *this;
}
