#pragma once
#include <array>
#include <vector>
#include "Entity.h"
#include "Event.h"
#include "Component.h"
#include "ComponentStorage.h"
#include "System.h"
#include "Registry.h"

namespace ecs {
	class Manager {
	public:
		Manager();
		~Manager();

		template<typename C>
		inline bool createComponentStore() {
			static_assert(std::is_base_of<Component, C>::value, "C must inherit from Component\n");

			return m_componentStores.insert(std::make_pair(getComponentTypeID<C>(), IComponentStore::Ptr(new ComponentStore<C>()))).second;
		}

		template<typename C>
		inline ComponentStore<C>& getComponentStore() {
			static_assert(std::is_base_of<Component, C>::value, "C must inherit from Component\n");

			auto iComponentStore = m_componentStores.find(getComponentTypeID<C>());
			if (iComponentStore == m_componentStores.end()) {
				throw std::runtime_error("Manager Error: This ComponentStore does not exist\n");
			}
			return reinterpret_cast<ComponentStore<C>&>(*(iComponentStore->second));
		}

		void addSystem(const System::Ptr& ptr, const std::string& name = std::string(""));

		System& getSystem(const std::string& name) {
			if (m_systemToNameMap.find(name) == m_systemToNameMap.end()) {
				throw std::runtime_error("Manager Error: This System does not exist\n");
			}
			return *m_systemToNameMap.at(name);
		}

		template<typename S>
		S& getSystem(const std::string& name) {
			static_assert(std::is_base_of<System, S>::value, "S must inherit from System\n");
			if (m_systemToNameMap.find(name) == m_systemToNameMap.end()) {
				throw std::runtime_error("Manager Error: This System does not exist\n");
			}
			return reinterpret_cast<S&>(m_systemToNameMap.at(name));
		}

		inline Entity::Handle& createEntity() {
			size_t i;
			for (i = 0; i < Entity::MAX_ENTITIES; i++) {
				if (!isAlive(i)) {
					break;
				}
			}
			if (i >= Entity::MAX_ENTITIES)
				throw std::runtime_error("Manager Error: Exceeded Maximum Entities\n");
			m_live.set(i, true);
			m_entityIDs[i] = i;
			m_entityKeybits[i] = 0;
			return m_entityIDs[i];
		}

		inline void destroyEntity(const Entity::Handle& entity) {
			if(isAlive(entity))
				m_live.set(entity, false);
		}

		inline bool isAlive(const Entity::Handle& entity) {
			return m_live.test(entity);
		}

		/// Moves component to storage
		/// Returns true if component is added
		///
		/// Throws std::runtime_error if entity is
		/// to be removed
		///
		/// Usage: addComponent<ComponentType>(const unsigned int&, std::move(Component))
		/// 
		/// Does not associate entity 
		/// with any systems
		template<typename C>
		inline bool addComponent(const Entity::Handle& entity, C&& component) {
			static_assert(std::is_base_of<Component, C>::value, "C must inherit from Component\n");
			if (!isAlive(entity))
				throw std::runtime_error("Manager Error: Cannot add component to \"" + std::to_string(entity) + "\". Entity does not exist\n");
			m_entityKeybits[entity].set(getComponentTypeID<C>());
			return getComponentStore<C>().add(entity, std::move(component));
		}
		
		/// Removes component type from storage
		/// Returns true if component is removed
		///
		/// Throws std::exception if entity is
		/// to be removed
		/// 
		/// Usage: removeComponent<ComponentType>(const int&)
		///
		/// Does not remove entity's 
		/// association with any systems
		/// 
		/// ** Warning: unregister/re-register
		/// entity to avoid crashes
		template<typename C>
		inline bool removeComponent(const Entity::Handle& entity) {
			static_assert(std::is_base_of<Component, C>::value, "C must inherit from Component\n");
			if (!isAlive(entity)) {
				throw std::exception("Manager Error: Cannot add component to \"" + std::to_string(entity) + "\". Entity does not exist\n");
				return false;
			}
			m_entityKeybits[entity].set(getComponentTypeID<C>(), false);
			return getComponentStore<C>().remove(entity);
		}
		
		/// Moves event to storage
		/// Returns true if event is added
		///
		/// Usage: injectEvent<EventType>(std::move(Event))
		template <typename E>
		inline void injectEvent(Event::Ptr ptr) {
			static_assert(std::is_base_of<Event, E>::value, "E must inherit from Event\n");
			std::unordered_map<EventType, std::vector<Event::Ptr>>::iterator it = m_events.find(getEventTypeID<E>());
			if (it == m_events.end()) {
				std::vector<Event::Ptr> tmp;
				tmp.push_back(std::move(ptr));
				m_events.insert(getEventTypeID<E>(), std::move(tmp));
			}
			else {
				(*it).second.push_back(std::move(ptr));
			}
		}

		/// Returns const Event::Ptr at the top of the vector
		/// without removing it
		///
		/// Usage: Event & event = peekEvent<EventType>()
		template <typename E>
		inline const Event::Ptr peekEvent() const {
			static_assert(std::is_base_of<Event, E>::value, "E must inherit from Event\n");
			std::unordered_map<EventType, std::vector<Event::Ptr>>::iterator it = m_events.find(getEventTypeID<E>());
			if (it != m_events.end()) {
				if ((*it).second.empty())
					return nullptr;
				tmp = (*it).second.at(0);
			}
			return tmp;
		}

		/// Returns const Event::Ptr at the top of the vector
		/// and removes it
		///
		/// Usage: Event & event = peekEvent<EventType>()
		template <typename E>
		inline const Event::Ptr extractEvent() const {
			static_assert(std::is_base_of<Event, E>::value, "E must inherit from Event\n");
			std::unordered_map<EventType, std::vector<Event::Ptr>>::iterator it = m_events.find(getEventTypeID<E>());
			if (it != m_events.end()) {
				if ((*it).second.empty()) {
					m_events.erase(it);
					return nullptr;
				}
				tmp = (*it).second.at(0);
				(*it).second.pop_back();
				return tmp;
			}
			return nullptr;
		}

		size_t registerEntity(const Entity::Handle& entity);
		size_t unregisterEntity(const Entity::Handle& entity);
		size_t UpdateEntities(const float& dt);

		void registerEntityName(const Entity::Handle& entity, const std::string& name, const std::string& path = "_GLOBAL");
		void unregisterEntityName(const std::string& name, const std::string& path = "_GLOBAL");
		void unregisterEntityName(const Entity::Handle& entity, const std::string& path = "_GLOBAL");
		
		const std::string&    getEntityName(const Entity::Handle& entity, const std::string& path = "_GLOBAL");
		const Entity::Handle& getEntityFromName(const std::string& name, const std::string& path = "_GLOBAL");


	private:
		std::bitset<Entity::MAX_ENTITIES>				m_live;
		std::array<Entity::Handle, Entity::MAX_ENTITIES>		m_entityIDs;
		std::array<Entity::Keybits, Entity::MAX_ENTITIES>		m_entityKeybits;
		std::unordered_map<ComponentType, IComponentStore::Ptr>		m_componentStores;
		std::vector<System::Ptr>					m_systems;
		std::unordered_map<std::string, System*>			m_systemToNameMap;
		std::unordered_map<EventType, std::vector<Event::Ptr>>		m_events;
		std::unordered_map<std::string, Registry>			m_registries;
	};
}
