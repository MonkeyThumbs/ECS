#include "../Include/Manager.h"

ecs::Manager::Manager() 
{
	createEntity();
	m_registries.insert(std::make_pair("_GLOBAL", Registry()));
}

ecs::Manager::~Manager() 
{}

void ecs::Manager::addSystem(const System::Ptr& ptr, const std::string& name) {
	if (!ptr || ptr->getLockbits() == 0) {
		throw std::runtime_error("Manager Error: System has no required Components specified\n");
	}

	if (name != "") {
		m_systemToNameMap.emplace(std::make_pair(name, ptr.get()));
	}

	m_systems.push_back(ptr);
}

size_t ecs::Manager::registerEntity(const Entity::Handle& entity) {
	size_t associatedSystems = 0;

	if (!m_live.test(entity)) {
		throw std::runtime_error("Manager Error: \"" + std::to_string(entity) + "\" cannot be registered. Entity does not exist\n");
	}
	auto keybits = m_entityKeybits.at(entity);
	for (auto system : m_systems) {
		auto lockbits = system->getLockbits();
		if (Compare(keybits, lockbits)) {
			system->registerEntity(entity);
			++associatedSystems;
		}
	}
	return associatedSystems;
}

size_t ecs::Manager::unregisterEntity(const Entity::Handle& entity) {
	size_t associatedSystems = 0;

	auto keybits = m_entityKeybits.at(entity);
	for (auto system : m_systems) {
		associatedSystems += system->unregisterEntity(entity);
	}
	return associatedSystems;
}

size_t ecs::Manager::UpdateEntities(const float& dt) {
	size_t updatedEntities = 0;
	for (size_t i = 0; i < Entity::MAX_ENTITIES; ++i) 
		if (!m_live.test(i) && m_entityIDs.at(i) != Entity::NULL_ENTITY) {
			unregisterEntity(i);
			m_entityKeybits[i].reset();
			m_entityIDs[i] = Entity::NULL_ENTITY;
		}
	for (auto& system : m_systems) 
		updatedEntities += system->UpdateEntities(dt);
	return updatedEntities;
}

void ecs::Manager::registerEntityName(const Entity::Handle& entity, const std::string& name, const std::string& path)
{
	if (m_registries.find(path) == m_registries.end())
		m_registries.insert(std::make_pair(path, Registry()));
	m_registries[path].Register(name, entity);
}

void ecs::Manager::unregisterEntityName(const std::string& name, const std::string& path)
{
	if (m_registries.find(path) != m_registries.end())
		m_registries[path].Unregister(name);
}

void ecs::Manager::unregisterEntityName(const Entity::Handle& entity, const std::string& path)
{
	if (m_registries.find(path) != m_registries.end())
		m_registries[path].Unregister(entity);
}

const std::string& ecs::Manager::getEntityName(const Entity::Handle& entity, const std::string& path)
{
	if (m_registries.find(path) == m_registries.end())
		throw std::runtime_error("Manager Error: path not found\n");
	return m_registries[path].Fetch(entity);
}

const ecs::Entity::Handle& ecs::Manager::getEntityFromName(const std::string& name, const std::string& path)
{
	if (m_registries.find(path) != m_registries.end())
		return m_registries[path].Fetch(name);
	else
		return Entity::NULL_ENTITY;
}
