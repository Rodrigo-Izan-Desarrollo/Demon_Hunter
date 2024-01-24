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
	if (app->lastScreen->isEnabled())
	{
		// Initialize the timer
		count = 0;

		// Textures
		Gameover = app->tex->Load("Assets/Screens/Menu_gameover.png");
		Missioncompleted = app->tex->Load("Assets/Screens/Completed_1.png");

		// Initialize fx
		over_theme = app->audio->LoadFx("Assets/Audio/Fx/Game_over.wav");// 6s
		win_theme = app->audio->LoadFx("Assets/Audio/Fx/Mission_completed.wav");// 4s

		// Disable the enable scenes
		app->guiManager->Disable();
		app->scene->Disable();
	}
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
		app->audio->PlayFx(over_theme);
	}
	else
	{
		currentTexture = Missioncompleted;
		app->audio->PlayFx(win_theme);
	}
	if (count > 375 && currentTexture == Gameover) {
		app->fade->StartFadeToBlack(this, (Module*)app->sceneMenu, 0);	
		app->lastScreen->Disable();
		app->sceneMenu->Enable();
	}
	if (count > 250 && currentTexture == Missioncompleted) {
		app->fade->StartFadeToBlack(this, (Module*)app->sceneMenu, 0);
		app->lastScreen->Disable();
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
	SDL_DestroyTexture(Gameover);
	SDL_DestroyTexture(Missioncompleted);

	//Unload Fx
	app->audio->UnloadFx(over_theme);
	app->audio->UnloadFx(win_theme);

	count = 0;

	return true;
}
