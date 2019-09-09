#pragma once

#include <string>

#include "core/ObjectBase.h"

class Resource : public ObjectBase
{
public:
	Resource(std::string InId);
	virtual ~Resource();

	virtual void Initialize() override;
	std::string GetResourceId();
protected:
	std::string Id;
private:
	Resource();
};
