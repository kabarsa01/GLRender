#pragma once

#include <string>

class HashString
{
public:
private:
	size_t HashValue;
	std::hash<std::string> Hash;
};
