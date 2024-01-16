#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager(App* application, bool start_enabled);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	//Create new funtions for getting agruping the entities
	void GetSlimes(List<Entity*>& slimesList) const;
	void GetBomber(List<Entity*>& slimesList) const;



public:

	List<Entity*> entities;

};

#endif // __ENTITYMANAGER_H__
