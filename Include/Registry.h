#pragma once

#include <unordered_map>
#include <string>

#include "Entity.h"

namespace ecs {
	class Registry
	{
	public:
		Registry() {}
		~Registry() {}

		bool Register(const std::string& name, Entity::Handle uid, bool override = true)
		{
			if (!override && m_registry.find(name) != m_registry.end()) {
				throw std::runtime_error(error_msg0);
				return false;
			}
			if (!m_registry.insert(std::make_pair(name, uid)).second)
				m_registry[name] = uid;
			return true;
		}

		void Unregister(const std::string& name) {
			if (m_registry.find(name) != m_registry.end())
				m_registry.erase(name);
			else
				throw std::runtime_error(error_msg1);
		}

		void Unregister(Entity::Handle uid) {
			for (auto& it : m_registry) 
				if (it.second == uid) {
					m_registry.erase(it.first);
					return;
				}
			throw std::runtime_error(error_msg1);
		}

		const Entity::Handle& Fetch(const std::string& name) {
			for (auto& it : m_registry)
				if (it.first == name) {
					return it.second;
				}
			throw std::runtime_error(error_msg1);
			return Entity::NULL_ENTITY;
		}

		const std::string& Fetch(const Entity::Handle& uid) {
			for (auto& it : m_registry)
				if (it.second == uid) {
					return it.first;
				}
			throw std::runtime_error(error_msg1);
			return error_msg1;
		}


	private:
		std::unordered_map<std::string, Entity::Handle> m_registry;

		std::string Registry::error_msg0 = "Registry Error: entity already exists.\n";
		std::string Registry::error_msg1 = "Registry Error: entity not found.\n";
	};
}
