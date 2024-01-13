#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "SceneMenu.h"
#include "LastScreen.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"

LastScreen::LastScreen(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneLogo");
}

// Destructor
LastScreen::~LastScreen()
{}

// Called before render is available
bool LastScreen::Awake()
{
	LOG("Loading Logo Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool LastScreen::Start()
{
	count = 0;
	// Textures
	Gameover = app->tex->Load("Assets/Screens/Menu_gameover.png");
	Missioncompleted = app->tex->Load("Assets/Screens/Completed_1.png");

	app->guiManager->Enable();
	app->scene->Disable();

	return true;
}

// Called each loop iteration
bool LastScreen::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool LastScreen::Update(float dt)
{
	if (app->scene->player->dead==true && app->scene->player->lifes<=0)
	{
		currentTexture = Gameover;
	}
	else
	{
		currentTexture = Missioncompleted;
	}
	if (count > 500) {
		app->fade->StartFadeToBlack(this, (Module*)app->sceneMenu, 0);
		app->sceneMenu->Enable();
	}
	else 
	{
		count++;
	}
	return true;
}

// Called each loop iteration
bool LastScreen::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(currentTexture, app->scene->player->position.x-125,app->scene->player->position.y - 550);
	return ret;
}

bool LastScreen::OnGuiMouseClickEvent(GuiControl* control) {
	return true;
}

// Called before quitting
bool LastScreen::CleanUp()
{
	LOG("Freeing best logo ever scene");

	//Hazme CleanUp de todo los que puedas
	app->tex->UnLoad(Gameover);
	app->tex->UnLoad(Missioncompleted);

	return true;
}
