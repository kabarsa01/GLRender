#pragma once

#include <string>
#include <memory>

#include "core/ObjectBase.h"
#include "common/HashString.h"

class Resource : public ObjectBase
{
public:
	Resource(HashString InId);
	virtual ~Resource();

	virtual void Initialize() override;
	HashString GetResourceId();

	virtual bool Load();
	bool IsValid();
protected:
	HashString Id;
	bool IsValidFlag = false;
private:
	Resource();
};

typedef std::shared_ptr<Resource> ResourcePtr;


