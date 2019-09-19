#pragma once

#include <memory>
#include <string>
#include <map>

// Hashed string stores it's hash value and uses one global hash table to store
// the actual strings. It means that there's only one instance of every string
// used as a hash string. All the comparisons are made using stored hash which
// makes them very cheap.
class HashString
{
public:
	HashString(std::string InString);
	virtual ~HashString();

	static HashString NONE();

	const size_t GetHash() const;
	const std::string& GetString() const;

	bool operator== (const HashString& rhs) const noexcept;
	bool operator!= (const HashString& rhs) const noexcept;
private:
	static std::map<size_t, std::shared_ptr<std::string>> StringsMap;

	size_t HashValue;
	std::shared_ptr< std::string > CachedString;

	HashString();
};
