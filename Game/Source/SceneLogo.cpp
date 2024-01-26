#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "SceneMenu.h"
#include "LastScreen.h"
#include "Scenepause.h"
#include "SceneSettings.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"

SceneLogo::SceneLogo(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("sceneLogo");
}

// Destructor
SceneLogo::~SceneLogo()
{}

// Called before render is available
bool SceneLogo::Awake()
{
	LOG("Loading Logo Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneLogo::Start()
{
	//Initialize the timer
	count = 0;

	//Textures
	Intro_1 = app->tex->Load("Assets/Screens/Intro_1.png");
	Intro_2 = app->tex->Load("Assets/Screens/Intro_2.png");

	//PLay Fx
	logo_theme = app->audio->LoadFx("Assets/Audio/Music/Sound_efect_logo.wav");
	app->audio->PlayFx(logo_theme);

	//Disable the enable scenes
	app->sceneMenu->Disable();
	app->sceneSettings->Disable();
	app->scene->Disable();
	app->scenePause->Disable();
	app->lastScreen->Disable();
	app->guiManager->Disable();

	//Initialize the current texture
	currentTexture = Intro_1;

	return true;
}

// Called each loop iteration
bool SceneLogo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLogo::Update(float dt)
{
	//Change the texture if the count is bigger than 220
	if (count > 220) {
		currentTexture = Intro_2;
	}
	//Change the scene if the count is bigger than 500
	if (count > 500) {
		//app->fade->FadeToBlackScene(this, (Module*)app->sceneMenu, 60.0f);
		app->sceneLogo->Disable();
		app->sceneMenu->Enable();
	}
	else {
		count++;
	}

	//Exit the game if the player press the scape key
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_UP) {
		return false;
	}

	//Change the scene if the player press the space key
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		//app->fade->FadeToBlackScene(this, (Module*)app->sceneMenu, 0.0f);
		app->sceneLogo->Disable();
		app->sceneMenu->Enable();
	}

	return true;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	// Draw everything --------------------------------------
	app->render->DrawTexture(currentTexture, 30,380);

	return ret;
}

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing best logo ever scene");

	//Unload textures
	SDL_DestroyTexture(Intro_1);
	SDL_DestroyTexture(Intro_2);

	//Unload Music
	app->audio->UnloadMusic("Assets/Audio/Music/Sound_efect_logo.wav");

	count = 0;

	return true;
}