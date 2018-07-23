#pragma once
#include <memory>
#include <unordered_map>
#include "Component.h"

namespace ecs {
	class IComponentStore {
	public:
		typedef std::unique_ptr<IComponentStore> Ptr;
	};

	template<typename C>
	class ComponentStore : public IComponentStore
	{
		static_assert(std::is_base_of<Component, C>::value, "C must inherit from Component\n");
	public:
		ComponentStore() {}
		~ComponentStore() {}

		inline bool	add(const Entity::Handle entity, C&& component) {
			if (!m_store.insert(std::make_pair(entity, std::move(component))).second) {
				m_store[entity] = std::move(component);
				return (m_store.find(entity) != m_store.end());
			}
			return true;
		}
		inline bool	remove(const Entity::Handle entity) {
			return (0 < m_store.erase(entity));
		}
		inline bool	has(Entity::Handle entity) {
			return (m_store.end() != m_store.find(entity));
		}
		inline C&	get(Entity::Handle entity) {
			return m_store.at(entity);
		}
		inline C	extract(Entity::Handle entity) {
			C component = m_store.at(entity);
			m_store.erase(entity);
			return component;
		}

		inline const std::unordered_map<Entity::Handle, Component>& all() {
			return m_store;
		}

		inline const size_t	size() {
			return m_store.size();
		}

	private:
		std::unordered_map<Entity::Handle, C> m_store;
	};
}