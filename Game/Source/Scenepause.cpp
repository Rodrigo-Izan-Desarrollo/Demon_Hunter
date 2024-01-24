#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "SceneMenu.h"
#include "Scenepause.h"
#include "LastScreen.h"
#include "Scene.h"
#include "SceneSettings.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"

ScenePause::ScenePause(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneLogo");
}

// Destructor
ScenePause::~ScenePause()
{}

// Called before render is available
bool ScenePause::Awake()
{
	LOG("Loading Logo Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ScenePause::Start()
{
	//Pause
	Pause_1 = app->tex->Load("Assets/Screens/Menu_pause_1.png");
	Pause_2 = app->tex->Load("Assets/Screens/Menu_pause_2.png");
	Pause_3 = app->tex->Load("Assets/Screens/Menu_pause_3.png");
	Pause_4 = app->tex->Load("Assets/Screens/Menu_pause_4.png");
	Pause_5 = app->tex->Load("Assets/Screens/Menu_pause_5.png");
	Pause_6 = app->tex->Load("Assets/Screens/Menu_pause_6.png");
	Pause_7 = app->tex->Load("Assets/Screens/Menu_pause_7.png");
	Pause_8 = app->tex->Load("Assets/Screens/Menu_pause_8.png");
	Pause_9 = app->tex->Load("Assets/Screens/Menu_pause_9.png");

	currentTexture = Pause_1;

	app->guiManager->Enable();
	app->scene->Disable();

	btn1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Title", {215, 69, 309, 106 }, this);
	btn2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Resume", { 253, 314, 509, 94 }, this);
	btn3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", {350, 485, 317, 50 }, this);
	btn4 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Exit", { 765, 64, 59, 57 }, this);


	//Habilita los botones
	btn1->state = GuiControlState::NORMAL;
	btn2->state = GuiControlState::NORMAL;
	btn4->state = GuiControlState::NORMAL;


	return true;
}

// Called each loop iteration
bool ScenePause::PreUpdate()
{
	if (btn1->state == GuiControlState::FOCUSED)
	{
		currentTexture = Pause_6;
	}
	if (btn1->state == GuiControlState::PRESSED)
	{
		currentTexture = Pause_7;
		app->fade->StartFadeToBlack(this, (Module*)app->sceneMenu);
		app->scenePause->Disable();
		app->sceneMenu->Enable();
	}

	if (btn2->state == GuiControlState::FOCUSED)
	{
		currentTexture = Pause_2;
	}
	if (btn2->state == GuiControlState::PRESSED)
	{
		currentTexture = Pause_3;
		app->fade->StartFadeToBlack(this, (Module*)app->scene);
		app->scenePause->Disable();
		app->scene->Enable();
	}

	if (btn3->state == GuiControlState::FOCUSED)
	{
		currentTexture = Pause_4;
	}
	if (btn3->state == GuiControlState::PRESSED)
	{
		currentTexture = Pause_5;
		app->fade->StartFadeToBlack(this, (Module*)app->sceneSettings);
		app->scenePause->Disable();
		app->sceneSettings->Enable();
	}

	if (btn4->state == GuiControlState::FOCUSED)
	{
		currentTexture = Pause_8;
	}
	if (btn4->state == GuiControlState::PRESSED)
	{
		currentTexture = Pause_9;
		return false;
	}

	return true;
}

// Called each loop iteration
bool ScenePause::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool ScenePause::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(currentTexture, app->scene->player->position.x - 125, app->scene->player->position.y - 550);

	return ret;
}

bool ScenePause::OnGuiMouseClickEvent(GuiControl* control) {

	return true;
}

// Called before quitting
bool ScenePause::CleanUp()
{
	LOG("Freeing best logo ever scene");

	SDL_DestroyTexture(Pause_1);
	SDL_DestroyTexture(Pause_2);
	SDL_DestroyTexture(Pause_3);
	SDL_DestroyTexture(Pause_4);
	SDL_DestroyTexture(Pause_5);
	SDL_DestroyTexture(Pause_6);
	SDL_DestroyTexture(Pause_7);
	SDL_DestroyTexture(Pause_8);
	SDL_DestroyTexture(Pause_9);

	app->guiManager->DestroyGuiControl(btn1);
	app->guiManager->DestroyGuiControl(btn2);
	app->guiManager->DestroyGuiControl(btn3);
	app->guiManager->DestroyGuiControl(btn4);

	return true;
}
