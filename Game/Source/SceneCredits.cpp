#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "SceneMenu.h"
#include "SceneCredits.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"

SceneCredits::SceneCredits(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneLogo");
}

// Destructor
SceneCredits::~SceneCredits()
{}

// Called before render is available
bool SceneCredits::Awake()
{
	LOG("Loading Logo Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneCredits::Start()
{
	// Textures
	Credits_1 = app->tex->Load("Assets/Screens/Credits_1.png");
	Credits_2 = app->tex->Load("Assets/Screens/Credits_2.png");

	currentTexture = Credits_1;

	app->guiManager->Enable();
	app->scene->Disable();

	btn1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Title", { 114, 95, 18, 17 }, this);

	//Habilita los botones
	btn1->state = GuiControlState::NORMAL;

	return true;
}

// Called each loop iteration
bool SceneCredits::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneCredits::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool SceneCredits::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(currentTexture, 30,380);

	return ret;
}

bool SceneCredits::OnGuiMouseClickEvent(GuiControl* control) {
	return true;
}

// Called before quitting
bool SceneCredits::CleanUp()
{
	LOG("Freeing best logo ever scene");

	//Hazme CleanUp de todo los que puedas
	app->tex->UnLoad(Credits_1);
	app->tex->UnLoad(Credits_2);

	app->guiManager->Disable();

	return true;
}
