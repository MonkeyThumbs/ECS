#include "Manager.h"

struct Test1 : ecs::Component {
};

struct Test2 : ecs::Component {
};

struct Test3 : ecs::Component {
};

struct PhysicsComponent : ecs::Component {
	PhysicsComponent(int* n) {
		testptr = n;
	}
	int* testptr;
};

class TestSystem : public ecs::System {
public:
	TestSystem(ecs::Manager& manager) : System(manager) {
		ecs::Entity::Lockbits lockbits;
		lockbits.set(ecs::getComponentTypeID<Test1>(), true);
		lockbits.set(ecs::getComponentTypeID<Test2>(), true);
		setLockbits(std::move(lockbits));
	}
	~TestSystem() {}

	virtual void UpdateEntity(const float& dt, ecs::Entity::Handle entity) override {}
};

class TestSystem2 : public ecs::System {
public:
	TestSystem2(ecs::Manager& manager) : System(manager) {
		ecs::Entity::Lockbits lockbits;
		lockbits.set(ecs::getComponentTypeID<Test2>(), true);
		lockbits.set(ecs::getComponentTypeID<Test3>(), true);
		setLockbits(std::move(lockbits));
	}
	~TestSystem2() {}

	virtual void UpdateEntity(const float& dt, ecs::Entity::Handle entity) override {}
};

class PhysicsSystem : public ecs::System
{
public:
	PhysicsSystem(ecs::Manager& manager) : System(manager) {
		ecs::Entity::Lockbits lockbits;
		lockbits.set(ecs::getComponentTypeID<PhysicsComponent>(), true);
		setLockbits(std::move(lockbits));
	}
	~PhysicsSystem() {}
	
	virtual void UpdateEntity(const float& dt, ecs::Entity::Handle entity) override
	{
		if (manager.getComponentStore<PhysicsComponent>().has(entity))
			return;
	}

private:
	float		m_ratio;
};

void main()
{
	ecs::Manager manager;

	manager.addSystem(ecs::System::Ptr(new TestSystem(manager)));
	manager.addSystem(ecs::System::Ptr(new TestSystem2(manager)));
	manager.addSystem(ecs::System::Ptr(new PhysicsSystem(manager)));
	manager.createComponentStore<Test1>();
	manager.createComponentStore<Test2>();
	manager.createComponentStore<Test3>();
	manager.createComponentStore<PhysicsComponent>();
	
	auto entity = manager.createEntity();
	manager.addComponent(entity, Test1());
	std::cout << std::to_string(manager.registerEntity(entity)) << std::endl;

	entity = manager.createEntity();
	manager.addComponent(entity, Test1());
	manager.addComponent(entity, Test2());
	std::cout << std::to_string(manager.registerEntity(entity)) << std::endl;

	entity = manager.createEntity();
	manager.addComponent(entity, Test1());
	manager.addComponent(entity, Test2());
	manager.addComponent(entity, Test3());
	std::cout << std::to_string(manager.registerEntity(entity)) << std::endl;

	manager.UpdateEntities(1);
	manager.destroyEntity(entity);
	manager.UpdateEntities(1);
}
