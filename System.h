#pragma once
#include <iostream>
#include <cassert>
#include <memory>
#include <set>
#include "Entity.h"
#include "Event.h"


namespace ecs {
	class Manager;

	inline bool Compare(const Entity::Keybits& keybits, const Entity::Lockbits& lockbits) {
		assert(keybits.size() == lockbits.size());
		for (size_t i = 0; i < keybits.size(); i++) {
			if (lockbits[i] == true && keybits[i] != true)
				return false;
		}
		return true;
	}

	class System {
	public:
		typedef std::shared_ptr<System> Ptr;

		explicit System(Manager& Manager);
		virtual ~System();

		inline const Entity::Lockbits& getLockbits() const {
			return m_lockbits;
		}
		inline bool registerEntity(Entity::Handle entity) {
			HandleRegistration(entity);
			return m_matchingEntities.insert(entity).second;
		}
		inline std::size_t unregisterEntity(Entity::Handle entity) {
			HandleUnregistration(entity);
			return m_matchingEntities.erase(entity);
		}
		inline bool hasEntity(Entity::Handle entity) {
			return (m_matchingEntities.end() != m_matchingEntities.find(entity));
		}
		inline void listenEvent(EventType type) {
			m_signature.set(type, true);
		}
		inline void ignoreEvent(EventType type) {
			m_signature.set(type, false);
		}
		inline bool isListening(EventType type) {
			return m_signature.test(type);
		}

		std::size_t UpdateEntities(const float& dt);
		virtual void HandleEvent(Event::Ptr ptr) {}

	protected:
		virtual void BeginUpdate() {}
		virtual void EndUpdate() {}
		virtual void UpdateEntity(const float& dt, const Entity::Handle& entity) = 0;

		virtual void HandleRegistration(const Entity::Handle& entity) {};
		virtual void HandleUnregistration(const Entity::Handle& entity) {};

		inline void setLockbits(Entity::Lockbits&& lockbits) {
			m_lockbits = std::move(lockbits);
		}
		inline void setSignature(Event::Signature&& signature) {
			m_signature = std::move(signature);
		}

		Manager&	manager;
		std::set<Entity::Handle>	m_matchingEntities;

	private:
		Entity::Lockbits			m_lockbits;
		Event::Signature			m_signature;
	};
}