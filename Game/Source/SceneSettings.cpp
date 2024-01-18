#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "SceneMenu.h"
#include "SceneSettings.h"
#include "LastScreen.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"

SceneSettings::SceneSettings(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneLogo");
}

// Destructor
SceneSettings::~SceneSettings()
{}

// Called before render is available
bool SceneSettings::Awake()
{
	LOG("Loading Logo Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneSettings::Start()
{
	// Soun textures
	Sound_1 = app->tex->Load("Assets/Textures/Menu/Sound_1.png");
	Sound_2 = app->tex->Load("Assets/Textures/Menu/Sound_2.png");
	Sound_3 = app->tex->Load("Assets/Textures/Menu/Sound_3.png");
	Sound_4 = app->tex->Load("Assets/Textures/Menu/Sound_4.png");
	Resta_1 = app->tex->Load("Assets/Textures/Menu/Resta_1.png");
	Resta_2 = app->tex->Load("Assets/Textures/Menu/Resta_2.png");
	Resta_3 = app->tex->Load("Assets/Textures/Menu/Resta_3.png");
	Suma_1 = app->tex->Load("Assets/Textures/Menu/Suma_1.png");
	Suma_2 = app->tex->Load("Assets/Textures/Menu/Suma_2.png");
	Suma_3 = app->tex->Load("Assets/Textures/Menu/Suma_3.png");

	currentTexture = Sound_1;

	app->guiManager->Enable();
	app->scene->Disable();

	btn1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Title", { 7, 30, 920, 290 }, this);
	btn2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Play", { 99, 339, 235, 90 }, this);
	btn3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Continue", { 92, 455, 254, 45 }, this);
	btn4 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Settings", { 112, 525, 213, 42 }, this);
	btn5 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Exit", { 0, 721, 51, 46 }, this);
	btn6 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Title", { 7, 30, 920, 290 }, this);

	//Habilita los botones
	btn1->state = GuiControlState::NORMAL;
	btn2->state = GuiControlState::NORMAL;
	btn4->state = GuiControlState::NORMAL;
	btn5->state = GuiControlState::NORMAL;
	btn6->state = GuiControlState::NORMAL;

	return true;
}

// Called each loop iteration
bool SceneSettings::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneSettings::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool SceneSettings::PostUpdate()
{
	bool ret = true;

	//app->render->DrawTexture(currentTexture, 30,380);

	app->render->DrawTexture(currentTexture, app->scene->player->position.x - 125, app->scene->player->position.y - 550);

	return ret;
}

bool SceneSettings::OnGuiMouseClickEvent(GuiControl* control) {

	return true;
}

// Called before quitting
bool SceneSettings::CleanUp()
{
	LOG("Freeing best logo ever scene");

	//Destruye los botones
	app->guiManager->Disable();

	return true;
}
