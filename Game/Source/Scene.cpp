#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Slime.h"
#include "BigSlime.h"
#include "Boss.h"
#include "SceneMenu.h"
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"

Scene::Scene(App* application, bool start_enabled) : Module(application, start_enabled)
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
	}
	slimesList;
	//Use the funtion
	app->entityManager->GetSlimes(slimesList);

	for (pugi::xml_node itemNode = config.child("bigslime"); itemNode; itemNode = itemNode.next_sibling("bigslime"))
	{
		bigslime = (BigSlime*)app->entityManager->CreateEntity(EntityType::BIGSLIME);
		bigslime->parameters = itemNode;
	}
	
	for (pugi::xml_node itemNode = config.child("boss"); itemNode; itemNode = itemNode.next_sibling("boss"))
	{
		boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
		boss->parameters = itemNode;
	}

	for (pugi::xml_node itemNode = config.child("slimevolador"); itemNode; itemNode = itemNode.next_sibling("slimevolador"))
	{
		slimevolador = (SlimeVolador*)app->entityManager->CreateEntity(EntityType::SLIMEVOLADOR);
		slimevolador->parameters = itemNode;
	 }
	vslimesList;
	//Use the funtion
	app->entityManager->GetBomber(vslimesList);
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
		//Pausa
	Pausa_1 = app->tex->Load("Assets/Textures/scene.png");
	Pausa_2 = app->tex->Load("Assets/Textures/scene.png");
	Pausa_3 = app->tex->Load("Assets/Textures/scene.png");
	Pausa_4 = app->tex->Load("Assets/Textures/scene.png");
	Pausa_5 = app->tex->Load("Assets/Textures/scene.png");
	Pausa_6 = app->tex->Load("Assets/Textures/scene.png");
	Pausa_7 = app->tex->Load("Assets/Textures/scene.png");
	Pausa_8 = app->tex->Load("Assets/Textures/scene.png");
	Pausa_9 = app->tex->Load("Assets/Textures/scene.png");
		//Settings
	Settings_1 = app->tex->Load("Assets/Textures/scene.png");
	Settings_2 = app->tex->Load("Assets/Textures/scene.png");
	Settings_3 = app->tex->Load("Assets/Textures/scene.png");
	Settings_4 = app->tex->Load("Assets/Textures/scene.png");
		//Gameover
	Gameover_1 = app->tex->Load("Assets/Textures/scene.png");
	    //Win
	Win_1 = app->tex->Load("Assets/Textures/scene.png");
		//Credits
	Credits_1 = app->tex->Load("Assets/Textures/scene.png");

	//Music
		//Game music
	if (app->scene->isEnabled())
	{
		app->audio->PlayMusic(configNode.child("music").attribute("musicpathambient").as_string());
	}


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

	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && player->save)
	{
		app->LoadRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveRequest();
		player->save = true;
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
	player->lifes = node.child("modes").attribute("lifes").as_int();

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
	for (int slimecount = 0; slimecount < slimesList.Count(); slimecount++) {

		// Retrieve the current slime entity from the list.
		Entity* slime = slimesList.At(slimecount)->data;

		// Convert the current count to a string for constructing XML attribute names.
		std::string count = std::to_string(slimecount + 1);

		// Update the position of the slime entity based on XML attributes.
		slime->position.x = node.child(("enemy" + count).c_str()).attribute("x").as_int();
		slime->position.y = node.child(("enemy" + count).c_str()).attribute("y").as_int();

		// Set the 'tp' (teleport) flag to true for the slime entity.
		slime->tp = true;
	}

	//Bomber
	//Same with bomber
	for (int vslimecount = 0; vslimecount < vslimesList.Count(); vslimecount++) {

		Entity* slimevolador = vslimesList.At(vslimecount)->data;

		std::string count = std::to_string(vslimecount + 1);
		slimevolador->position.x = node.child(("venemy" + count).c_str()).attribute("x").as_int();
		slimevolador->position.y = node.child(("venemy" + count).c_str()).attribute("y").as_int();
		slimevolador->tp = true;
	}

	return true;
}

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
	modesnode.append_attribute("lifes").set_value(player->lifes);

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
	
	for (int slimecount = 0; slimecount < slimesList.Count(); slimecount++) {

		// Convert the current count to a string for constructing XML attribute names.
		std::string hola = std::to_string(slimecount + 1);

		// Append a child node for the current slime entity to the XML node.
		pugi::xml_node enemyNode = node.append_child(("enemy" + hola).c_str());

		// Retrieve the current slime entity from the list.
		Entity* slime = slimesList.At(slimecount)->data;

		// Load specific information of the slime from the attributes of the nodes.
		enemyNode.append_attribute("x").set_value(slime->position.x);
		enemyNode.append_attribute("y").set_value(slime->position.y);
	}

	//Bomber
	for (int vslimecount = 0; vslimecount < vslimesList.Count(); vslimecount++) {

		std::string count = std::to_string(vslimecount + 1);
		pugi::xml_node enemyNode = node.append_child(("venemy" + count).c_str());

		Entity* slimevolador = vslimesList.At(vslimecount)->data;

		enemyNode.append_attribute("x").set_value(slimevolador->position.x);
		enemyNode.append_attribute("y").set_value(slimevolador->position.y);
	}

	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}
