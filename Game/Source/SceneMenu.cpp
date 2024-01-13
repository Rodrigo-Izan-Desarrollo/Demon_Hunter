#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneMenu.h"
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
	
	//PLay Fx
	logo_theme = app->audio->LoadFx("Assets/Audio/Music/Sound_efect_logo.ogg");
	app->audio->PlayFx(logo_theme);

	currentTexture = Menu_1;

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


	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(currentTexture, 30,380);

	return ret;
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