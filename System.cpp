#include "System.h"
#include "Manager.h"

ecs::System::System(Manager& manager) 
:	manager(manager) 
{}

ecs::System::~System() 
{}

size_t ecs::System::UpdateEntities(const float& dt) {
	size_t updatedEntities = 0;
	BeginUpdate();
	for (auto& it : m_matchingEntities) {
		UpdateEntity(dt, it);
		++updatedEntities;
	}
	EndUpdate();
	return updatedEntities;
}