#include "EntityManager.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::SLIME:
		entity = new Slime();
		break;
	case EntityType::BIGSLIME:
		entity = new BigSlime();
		break;
	case EntityType::BOSS:
		entity = new Boss();
		break;
	case EntityType::SKELETON:
		entity = new Skeleton();
		break;
	case EntityType::SLIMEVOLADOR:
		entity = new SlimeVolador();
		break;
	case EntityType::POWERUP_1:
		entity = new Powerup_1();
		break;
	case EntityType::POWERUP_2:
		entity = new Powerup_2();
		break;
	case EntityType::POWERUP_3:
		entity = new Powerup_3();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

void EntityManager::GetSlimes(List<Entity*>& slimesList) const
{
	// Clear the provided list to ensure it's empty before populating it.
	slimesList.Clear();

	// Iterate through the list of entities.
	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		// Check if the current entity is of type "SLIME".
		if (entity->data->type == EntityType::SLIME)
		{
			// If it is a slime, add it to the provided list.
			slimesList.Add(entity->data);
		}
	}
}
//Same with slimevolador
void EntityManager::GetBomber(List<Entity*>& slimesList) const
{
	slimesList.Clear();

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		if (entity->data->type == EntityType::SLIMEVOLADOR)
		{
			slimesList.Add(entity->data);
		}
	}
}