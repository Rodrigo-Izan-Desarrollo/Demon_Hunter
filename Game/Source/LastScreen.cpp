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

		// Initialize fx
		over_theme = app->audio->LoadFx("Assets/Audio/Fx/Game_over.wav");// 6s
		win_theme = app->audio->LoadFx("Assets/Audio/Fx/Mission_completed.wav");// 4s

		if (app->scene->boss->death)
		{
			app->audio->PlayFx(win_theme);
		}
		if (app->scene->player->dead)
		{
			app->audio->PlayFx(over_theme);
		}

		currentTexture = Gameover;
		// Disable the enable scenes
		app->guiManager->Disable();
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
	if (count > 375 && app->scene->player->dead) {

		return false;
	}
	if (count> 250 && app->scene->boss->death) {
		return false;
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
	if (app->scene->player->dead)
	{
		app->render->DrawTexture(currentTexture, app->scene->player->position.x-125, app->scene->player->position.y-550);
	}

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
