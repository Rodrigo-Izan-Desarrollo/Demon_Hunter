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
#include "SceneSettings.h"
#include "LastScreen.h"
#include "ScenePause.h"
#include "FadeToBlack.h"
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"

#include "SDL_mixer/include/SDL_mixer.h"

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
	bslimesList;
	//Use the funtion
	app->entityManager->GetBigSlime(bslimesList);
	
	for (pugi::xml_node itemNode = config.child("boss"); itemNode; itemNode = itemNode.next_sibling("boss"))
	{
		boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
		boss->parameters = itemNode;
	}
	bossList;
	//Use the funtion
	app->entityManager->GetBoss(bossList);

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

	if (app->scene->isEnabled())
	{
		//Textures
		Portal = app->tex->Load("Assets/Maps/Portales.png");
		Carteles = app->tex->Load("Assets/Maps/Carteles.png");
		Checkpoint = app->tex->Load("Assets/Maps/Checkpoint.png");

		
		//Animations
		checkpoint.PushBack({ 0,0,32,32 });
		checkpoint.PushBack({ 32,0,32,32 });
		checkpoint.PushBack({ 64,0,32,32 });
		checkpoint.PushBack({ 96,0,32,32 });
		checkpoint.PushBack({ 128,0,32,32 });
		checkpoint.speed = 0.15f;
		checkpoint.loop = false;

		checkpoint2.PushBack({ 0,0,32,32 });
		checkpoint2.loop = false;

		checkpoint_2.PushBack({ 0,0,32,32 });
		checkpoint_2.PushBack({ 32,0,32,32 });
		checkpoint_2.PushBack({ 64,0,32,32 });
		checkpoint_2.PushBack({ 96,0,32,32 });
		checkpoint_2.PushBack({ 128,0,32,32 });
		checkpoint_2.speed = 0.15f;
		checkpoint_2.loop = false;

		checkpoint2_2.PushBack({ 0,0,32,32 });
		checkpoint2_2.loop = false;

		portalAnim.PushBack({ 0,14,38,36 });
		portalAnim.PushBack({ 48,14,38,36 });
		portalAnim.PushBack({ 96,14,38,36 });
		portalAnim.PushBack({ 144,14,38,36 });
		portalAnim.speed = 0.125f;
		portalAnim.loop = true;

		Q.PushBack({ 0,0,40,50 });
		Q.PushBack({ 44,0,40,50 });
		Q.speed= 0.2f;
		Q.loop = true;

		Click.PushBack({ 88,0,31,50 });
		Click.PushBack({ 123,0,31,50 });
		Click.speed = 0.2f;
		Click.loop = true;

		Arrow.PushBack({ 158,0,37,50 });
		Arrow.PushBack({ 199,0,37,50 });
		Arrow.speed = 0.2f;
		Arrow.loop = true;

		One.PushBack({ 240,0,37,50 });
		One.PushBack({ 281,0,37,50 });
		One.speed = 0.2f;
		One.loop = true;

		LSHIFT.PushBack({ 322,0,66,50 });
		LSHIFT.PushBack({ 390,0,66,50 });
		LSHIFT.speed = 0.2f;
		LSHIFT.loop = true;

		Two.PushBack({ 462,0,37,50 });
		Two.PushBack({ 503,0,35,50 });
		Two.speed = 0.2f;
		Two.loop = true;

		E.PushBack({ 544,0,37,50 });
		E.PushBack({ 585,0,37,50 });
		E.speed = 0.2f;
		E.loop = true;

		Three.PushBack({ 626,0,37,50 });
		Three.PushBack({ 667,0,37,50 });
		Three.speed = 0.2f;
		Three.loop = true;

		R.PushBack({ 708,0,37,50 });
		R.PushBack({ 749,0,37,50 });
		R.speed = 0.2f;
		R.loop = true;
		
		
		//Music
			//Game music
		app->audio->PlayMusic(configNode.child("music").attribute("musicpathambient").as_string());
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

		currentportal = &portalAnim;
		currentcheckpoint = &checkpoint2;
		currentcheckpoint2 = &checkpoint2_2;
		currentQ = &Q;
		currentClick = &Click;
		currentOne = &One;
		currentLSHIFT = &LSHIFT;
		currentTwo = &Two;
		currentE = &E;
		currentThree = &Three;
		currentR = &R;
		currentArrow = &Arrow;
	}

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
	currentportal->Update();
	currentcheckpoint->Update();
	currentQ->Update();
	currentClick->Update();
	currentOne->Update();
	currentLSHIFT->Update();
	currentTwo->Update();
	currentE->Update();
	currentThree->Update();
	currentR->Update();
	currentArrow->Update();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && player->save)
	{
		app->LoadRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveRequest();
		player->save = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		Mix_VolumeMusic(0);
	}

	if (player->dead==true && player->lifes<=0)
	{
		app->fade->StartFadeToBlack(this, (Module*)app->lastScreen, 0);
		app->scene->Disable();
		app->lastScreen->Enable();
	}
	
	if (app->sceneMenu->newgame == true)
	{
		player->position.x = 140;
		player->position.y = 925;
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y) }, 0);
		app->sceneMenu->newgame = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		app->fade->StartFadeToBlack(this, (Module*)app->scenePause, 10);
		app->scene->Disable();
		app->scenePause->Enable();
		pausa = true;
	}


	if (player->check_1)
	{
		currentcheckpoint = &checkpoint;
	}
	if (player->check_2)
	{
		currentcheckpoint2 = &checkpoint_2;
	}


	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	//Draw texture
	app->render->DrawTexture(Portal, 3328, 1532, &currentportal->GetCurrentFrame());
	app->render->DrawTexture(Portal, 4672, 1310, &currentportal->GetCurrentFrame());
	app->render->DrawTexture(Portal, 8660, 92, &currentportal->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);

	app->render->DrawTexture(Carteles, 928, 832, &currentQ->GetCurrentFrame());
	app->render->DrawTexture(Carteles, 980, 832, &currentClick->GetCurrentFrame());

	app->render->DrawTexture(Carteles, 3328, 1350, &currentOne->GetCurrentFrame());
	app->render->DrawTexture(Carteles, 3392, 1350, &currentLSHIFT->GetCurrentFrame());

	app->render->DrawTexture(Carteles, 4704, 1255, &currentTwo->GetCurrentFrame());
	app->render->DrawTexture(Carteles, 4768, 1255, &currentE->GetCurrentFrame());

	app->render->DrawTexture(Carteles, 8608, 40, &currentThree->GetCurrentFrame());
	app->render->DrawTexture(Carteles, 8672, 40, &currentR->GetCurrentFrame());

	app->render->DrawTexture(Carteles, 4192, 992, &currentArrow->GetCurrentFrame());
	app->render->DrawTexture(Carteles, 5120, 928, &currentArrow->GetCurrentFrame());
	app->render->DrawTexture(Carteles, 7136, 960, &currentArrow->GetCurrentFrame());

	app->render->DrawTexture(Checkpoint, 3392, 1056, &currentcheckpoint->GetCurrentFrame());
	app->render->DrawTexture(Checkpoint, 6784, 992, &currentcheckpoint2->GetCurrentFrame());

	return ret;
}

iPoint Scene::GetPLayerPosition() {

	return player->position;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	//Unload music
	app->audio->UnloadMusic(configNode.child("music").attribute("musicpathambient").as_string());

	//Destruye las texturas
	SDL_DestroyTexture(Portal);
	SDL_DestroyTexture(Carteles);
	SDL_DestroyTexture(Checkpoint);

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
	player->save = node.child("modes").attribute("save").as_bool();

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
		slime->death = node.child(("slime" + count).c_str()).attribute("death").as_bool();
		slime->position.x = node.child(("enemy" + count).c_str()).attribute("x").as_int();
		slime->position.y = node.child(("enemy" + count).c_str()).attribute("y").as_int();

		// Set the 'tp' (teleport) flag to true for the slime entity.
		slime->tp = true;
	}

	//Bomber
	for (int vslimecount = 0; vslimecount < vslimesList.Count(); vslimecount++) {

		Entity* slimevolador = vslimesList.At(vslimecount)->data;

		std::string count = std::to_string(vslimecount + 1);
		slimevolador->position.x = node.child(("venemy" + count).c_str()).attribute("x").as_int();
		slimevolador->position.y = node.child(("venemy" + count).c_str()).attribute("y").as_int();
		slimevolador->tp = true;
	}

	//Boss
	for (int bosscount = 0; bosscount < bossList.Count(); bosscount++) {

		Entity* boss = bossList.At(bosscount)->data;

		std::string count = std::to_string(bosscount + 1);
		boss->position.x = node.child(("boss" + count).c_str()).attribute("x").as_int();
		boss->position.y = node.child(("boss" + count).c_str()).attribute("y").as_int();
		boss->tp = true;
	}

	//BigSlime
	for (int bigslimecount = 0; bigslimecount < bslimesList.Count(); bigslimecount++) {

		Entity* bigslime = bslimesList.At(bigslimecount)->data;

		std::string count = std::to_string(bigslimecount + 1);
		bigslime->position.x = node.child(("bigslime" + count).c_str()).attribute("x").as_int();
		bigslime->position.y = node.child(("bigslime" + count).c_str()).attribute("y").as_int();
		bigslime->tp = true;
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
	modesnode.append_attribute("save").set_value(player->save);

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
		enemyNode.append_attribute("death").set_value(slime->death);
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

	//Boss
	for (int bosscount = 0; bosscount < bossList.Count(); bosscount++) {

		std::string count = std::to_string(bosscount + 1);
		pugi::xml_node enemyNode = node.append_child(("boss" + count).c_str());

		Entity* boss = bossList.At(bosscount)->data;

		enemyNode.append_attribute("x").set_value(boss->position.x);
		enemyNode.append_attribute("y").set_value(boss->position.y);
	}

	//BigSlime
	for (int bigslimecount = 0; bigslimecount < bslimesList.Count(); bigslimecount++) {

		std::string count = std::to_string(bigslimecount + 1);
		pugi::xml_node enemyNode = node.append_child(("bigslime" + count).c_str());

		Entity* bigslime = bslimesList.At(bigslimecount)->data;

		enemyNode.append_attribute("x").set_value(bigslime->position.x);
		enemyNode.append_attribute("y").set_value(bigslime->position.y);
	}

	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	return true;
}
