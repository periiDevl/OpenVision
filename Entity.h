#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include "UUID.h"

class Entity
{

public:
	Entity() : m_entityID(UUIDManager::randomUUID())
	{ }
	Entity(const std::string& name) : m_name(name), m_entityID(UUIDManager::randomUUID())
	{ }

	std::string name() const;

private:
	std::string m_name;
	UUID m_entityID;

};



#endif