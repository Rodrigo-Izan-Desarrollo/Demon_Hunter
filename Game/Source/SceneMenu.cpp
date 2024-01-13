#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "SceneMenu.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"

SceneMenu::SceneMenu(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneLogo");
}

// Destructor
SceneMenu::~SceneMenu()
{}

// Called before render is available
bool SceneMenu::Awake()
{
	LOG("Loading Logo Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneMenu::Start()
{
	// Textures
	Menu_1 = app->tex->Load("Assets/Screens/Menu_intro_1.png");
	Menu_2 = app->tex->Load("Assets/Screens/Menu_intro_2.png");
	Menu_3 = app->tex->Load("Assets/Screens/Menu_intro_3.png");
	Menu_4 = app->tex->Load("Assets/Screens/Menu_intro_4.png");
	Menu_5 = app->tex->Load("Assets/Screens/Menu_intro_5.png");
	Menu_6 = app->tex->Load("Assets/Screens/Menu_intro_6.png");
	Menu_7 = app->tex->Load("Assets/Screens/Menu_intro_7.png");
	Menu_8 = app->tex->Load("Assets/Screens/Menu_intro_8.png");
	Menu_9 = app->tex->Load("Assets/Screens/Menu_intro_9.png");
	Menu_10 = app->tex->Load("Assets/Screens/Menu_intro_10.png");
	Menu_11 = app->tex->Load("Assets/Screens/Menu_intro_11.png");
	Menu_12 = app->tex->Load("Assets/Screens/Menu_intro_12.png");

	currentTexture = Menu_6;

	app->scene->Disable();
	app->entityManager->Disable();
	app->map->Disable();
	app->guiManager->Enable();

	btn1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Title", { 7, 30, 920, 290 }, this);
	btn2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Play", { 99, 339, 235, 90 }, this);
	btn3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Continue", { 92, 455, 254, 45 }, this);
	btn4 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Settings", { 112, 525, 213, 42 }, this);
	btn5 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Exit", { 0, 721, 51, 46 }, this);

	return true;
}

// Called each loop iteration
bool SceneMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMenu::Update(float dt)
{
	if (app->scene->player->save==true)
	{
		currentTexture = Menu_1;
	}

	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(currentTexture, 0,380);

	return ret;
}

bool SceneMenu::OnGuiMouseClickEvent(GuiControl* control) {
	return true;
}

// Called before quitting
bool SceneMenu::CleanUp()
{
	LOG("Freeing best logo ever scene");

	//app->tex->UnLoad(Intro_1);
	//app->tex->UnLoad(Intro_2);
	//app->audio->UnloadFx(logo_theme);

	return true;
}
