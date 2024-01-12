#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
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
	// Textures
	count = 0;
	Intro_1 = app->tex->Load("Assets/Screens/Intro_2.png");
	
	// Audio

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
	if (count > 1000) {
		app->fade->StartFadeToBlack(this, (Module*)app->scene, 0);
	}
	else {
		count++;
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_UP) {
		return false;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		app->audio->PlayFx(fxEnter);
		app->fade->StartFadeToBlack(this, (Module*)app->scene, 0);
	}

	return true;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(Intro_1, 30,380);
	fxEnter = 0;

	return ret;
}

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing best logo ever scene");

	app->tex->UnLoad(Intro_1);

	return true;
}