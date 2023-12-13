#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("slime"); itemNode; itemNode = itemNode.next_sibling("slime"))
	{
		slime = (Slime*)app->entityManager->CreateEntity(EntityType::SLIME);
		slime->parameters = itemNode;
		//app->entityManager->GetSlimes();
	}
	for (pugi::xml_node itemNode = config.child("skeleton"); itemNode; itemNode = itemNode.next_sibling("skeleton"))
	{
		skeleton = (Skeleton*)app->entityManager->CreateEntity(EntityType::SKELETON);
		skeleton->parameters = itemNode;
	}

	for (pugi::xml_node itemNode = config.child("bomber"); itemNode; itemNode = itemNode.next_sibling("bomber"))
	{
		bomber = (Bomber*)app->entityManager->CreateEntity(EntityType::BOMBER);
		bomber->parameters = itemNode;
	 }
  
	for (pugi::xml_node itemNode = config.child("powerup_1"); itemNode; itemNode = itemNode.next_sibling("powerup_1"))
	{
		powerup_1 = (Powerup_1*)app->entityManager->CreateEntity(EntityType::POWERUP_1);
		powerup_1->parameters = itemNode;
	}
  
	for (pugi::xml_node itemNode = config.child("powerup_2"); itemNode; itemNode = itemNode.next_sibling("powerup_2"))
	{
		powerup_2 = (Powerup_2*)app->entityManager->CreateEntity(EntityType::POWERUP_2);
		powerup_2->parameters = itemNode;
	}
  

	for (pugi::xml_node itemNode = config.child("powerup_3"); itemNode; itemNode = itemNode.next_sibling("powerup_3"))
	{
		powerup_3 = (Powerup_3*)app->entityManager->CreateEntity(EntityType::POWERUP_3);
		powerup_3->parameters = itemNode;
    
	}

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	if (config.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();
	}

	configNode = config;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	/*app->audio->PlayMusic(configNode.child("music").attribute("musicpathambient").as_string());*/

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	float camSpeed = 1; 

	//if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//	app->render->camera.y -= (int)ceil(camSpeed * dt);

	//if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//	app->render->camera.y += (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	//// Renders the image in the center of the screen 
	////app->render->DrawTexture(img, (int)textPosX, (int)textPosY);
	/*iPoint mousePos;
	app->input->GetMousePosition(mousePos.x, mousePos.y);

	iPoint mouseTile = app->map->WorldToMap(mousePos.x - app->render->camera.x,
		mousePos.y - app->render->camera.y);

	app->map->pathfinding->CreatePath(mouseTile, app->map->WorldToMap(player->position.x, player->position.y));*/

	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveRequest();
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

iPoint Scene::GetPLayerPosition() {
	return player->position;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::LoadState(pugi::xml_node node) {

	//Player 

		//Player pos
	player->position.x= node.child("playerposition").attribute("x").as_int();
	player->position.y = node.child("playerposition").attribute("y").as_int();
	player->pbody->body->SetTransform({ PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y) }, 0);

		//Player-modes
	player->sleeping = node.child("modes").attribute("sleeping").as_bool();
	player->dead = node.child("modes").attribute("dead").as_bool();
	player->canmove = node.child("modes").attribute("canmove").as_bool();
	player->inground = node.child("modes").attribute("inground").as_bool();
	player->rightmode = node.child("modes").attribute("rightmode").as_bool();
	player->leftmode = node.child("modes").attribute("leftmode").as_bool();
	player->caninv = node.child("modes").attribute("caninv").as_bool();
	player->Godmode = node.child("modes").attribute("God-mode").as_bool();

		//Player canpower-ups
	player->canpower_1 = node.child("canpower").attribute("canpower-1").as_bool();
	player->canpower_2 = node.child("canpower").attribute("canpower-2").as_bool();
	player->canpower_3 = node.child("canpower").attribute("canpower-3").as_bool();

		//Player power-ups
	player->powerup_1 = node.child("power").attribute("power-1").as_bool();
	player->powerup_2 = node.child("power").attribute("power-2").as_bool();
	player->powerup_3 = node.child("power").attribute("power-3").as_bool();
	player->canchange = node.child("power").attribute("canchange").as_bool();

	//Items

		//Power-ups
	powerup_1->isPicked = node.child("poweritem").attribute("poweritem-1").as_bool();
	powerup_2->isPicked = node.child("poweritem").attribute("poweritem-2").as_bool();
	powerup_3->isPicked = node.child("poweritem").attribute("poweritem-3").as_bool();

	//Slime

		//Slime pos
	slime->position.x = node.child("slimepos").attribute("x").as_int();
	slime->position.y = node.child("slimepos").attribute("y").as_int();
	slime->pbody->body->SetTransform({ PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y) }, 0);

		//Slime modes
	slime->death = node.child("modess").attribute("dead").as_bool();
	slime->leftmodeslime = node.child("modess").attribute("leftmode").as_bool();
	slime->rightmodeslime = node.child("modess").attribute("rightmode").as_bool();

	return true;
}
// L14: TODO 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool Scene::SaveState(pugi::xml_node node) {

	//Append on nodes para todo lo que tiene que ver que las entidades del mapa
	
	//Player

		//Player pos
	pugi::xml_node posnode = node.append_child("playerposition");
	posnode.append_attribute("x").set_value(player->position.x);
	posnode.append_attribute("y").set_value(player->position.y);

		//Player-modes
	pugi::xml_node modesnode = node.append_child("modes");
	modesnode.append_attribute("sleeping").set_value(player->sleeping);
	modesnode.append_attribute("dead").set_value(player->dead);
	modesnode.append_attribute("canmove").set_value(player->canmove);
	modesnode.append_attribute("inground").set_value(player->inground);
	modesnode.append_attribute("rightmode").set_value(player->rightmode);
	modesnode.append_attribute("leftmode").set_value(player->leftmode);
	modesnode.append_attribute("caninv").set_value(player->caninv);
	modesnode.append_attribute("God-mode").set_value(player->Godmode);

		//Player canpower-ups
	pugi::xml_node canpowernode = node.append_child("canpower");
	canpowernode.append_attribute("canpower-1").set_value(player->canpower_1);
	canpowernode.append_attribute("canpower-2").set_value(player->canpower_2);
	canpowernode.append_attribute("canpower-3").set_value(player->canpower_3);

		//Player powerups
	pugi::xml_node powernode = node.append_child("power");
	powernode.append_attribute("power-1").set_value(player->powerup_1);
	powernode.append_attribute("power-2").set_value(player->powerup_2);
	powernode.append_attribute("power-3").set_value(player->powerup_3);
	powernode.append_attribute("canchange").set_value(player->canchange);

	//Items 

		//Powerups
	pugi::xml_node itempowernode = node.append_child("poweritem");
	itempowernode.append_attribute("poweritem-1").set_value(powerup_1->isPicked);
	itempowernode.append_attribute("poweritem-2").set_value(powerup_2->isPicked);
	itempowernode.append_attribute("poweritem-3").set_value(powerup_3->isPicked);


	//Slime
	
		//Slime pos
	pugi::xml_node posslimenode = node.append_child("slimepos");
	posslimenode.append_attribute("x").set_value(slime->position.x);
	posslimenode.append_attribute("y").set_value(slime->position.y);

		//Slime modes
	pugi::xml_node modesnodes = node.append_child("modess");
	modesnodes.append_attribute("dead").set_value(slime->death);
	modesnodes.append_attribute("leftmode").set_value(slime->leftmodeslime);
	modesnodes.append_attribute("rightmode").set_value(slime->rightmodeslime);

	return true;
}

