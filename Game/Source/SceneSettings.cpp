#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "SceneMenu.h"
#include "SceneSettings.h"
#include "Scenepause.h"
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
	Sound_1 = app->tex->Load("Assets/Screens/Menu_sound_1.png");
	Sound_2 = app->tex->Load("Assets/Screens/Menu_sound_2.png");
	Sound_3 = app->tex->Load("Assets/Screens/Menu_sound_3.png");
	Sound_4 = app->tex->Load("Assets/Screens/Menu_sound_4.png");
	Resta_1 = app->tex->Load("Assets/Screens/Resta_1.png");
	Resta_2 = app->tex->Load("Assets/Screens/Resta_2.png");
	Resta_3 = app->tex->Load("Assets/Screens/Resta_3.png");
	Suma_1 = app->tex->Load("Assets/Screens/Suma_1.png");
	Suma_2 = app->tex->Load("Assets/Screens/Suma_2.png");
	Suma_3 = app->tex->Load("Assets/Screens/Suma_3.png");	

	currentTexture = Sound_1;
	currentTextureresta_1 = Resta_1;
	currentTexturesuma_1 = Suma_1;
	currentTexturesuma_2 = Suma_1;
	currentTextureresta_2 = Resta_1;

	app->guiManager->Enable();
	app->scene->Disable();

	btn1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Fullscreen", {660, 371, 117, 117 }, this);
	btn2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Vsync", { 660, 514, 117, 117 }, this);
	btn3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Suma2", {730, 241, 117, 117 }, this);
	btn4 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Resta2", { 544, 241, 117, 117 }, this);
	btn5 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Suma1", {730, 86, 117, 117 }, this);
	btn6 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Resta1", { 544, 86, 117, 117 }, this);

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
	currentTexture = Sound_1;
	currentTextureresta_1 = Resta_1;
	currentTexturesuma_1 = Suma_1;
	currentTexturesuma_2 = Suma_1;
	currentTextureresta_2 = Resta_1;

	if (!app->scene->pausa)
	{
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			app->fade->StartFadeToBlack(this, (Module*)app->sceneMenu, 10);
			app->sceneSettings->Disable();
			app->sceneMenu->Enable();
		}
	}
	else
	{
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			app->fade->StartFadeToBlack(this, (Module*)app->scenePause, 10);
			app->sceneSettings->Disable();
			app->scenePause->Enable();
		}
	}

	if (btn6->state == GuiControlState::FOCUSED)
	{
		currentTextureresta_1 = Resta_2;
	}
	if (btn6->state == GuiControlState::PRESSED)
	{
		currentTextureresta_1 = Resta_3;
		app->audio->ChangeVolume(-10.0f);
		LOG("Volumen: %f", app->audio->GetVolume());
	}
	if (btn4->state == GuiControlState::FOCUSED)
	{
		currentTextureresta_2 = Resta_2;
	}
	if (btn4->state == GuiControlState::PRESSED)
	{
		currentTextureresta_2 = Resta_3;
		app->audio->ChangeFXVolume(-10.0f);
	}

	if (btn5->state == GuiControlState::FOCUSED)
	{
		currentTexturesuma_1 = Suma_2;
	}
	if (btn5->state == GuiControlState::PRESSED)
	{
		currentTexturesuma_1 = Suma_3;
		app->audio->ChangeVolume(+10.0f);
	}
	if (btn3->state == GuiControlState::FOCUSED)
	{
		currentTexturesuma_2 = Suma_2;
	}
	if (btn3->state == GuiControlState::PRESSED)
	{
		currentTexturesuma_2 = Suma_3;
		app->audio->ChangeFXVolume(+10.0f);
	}

	if (btn1->state == GuiControlState::PRESSED)
	{
		fullscreen = !fullscreen;
		app->win->fullscreen_window= !app->win->fullscreen_window;
	}
	if (btn2->state == GuiControlState::PRESSED)
	{
		vsync = !vsync;
		app->vsync = !app->vsync;
	}

	if (fullscreen && !vsync)
	{
		currentTexture = Sound_2;
		app->win->fullscreen_window = true;
		app->win->ToggleFullscreen();
		
	}
	if (!fullscreen)
	{
		app->win->fullscreen_window	=false;
		app->win->ToggleFullscreen();
	}
	if (vsync && !fullscreen)
	{
		currentTexture = Sound_3;
	}
	if (vsync && fullscreen)
	{
		currentTexture = Sound_4;
	}

	return true;
}

// Called each loop iteration
bool SceneSettings::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(currentTexture, app->scene->player->position.x - 125, app->scene->player->position.y - 550);

	app->render->DrawTexture(currentTextureresta_1, app->scene->player->position.x + 419, app->scene->player->position.y - 464);
	app->render->DrawTexture(currentTexturesuma_1,app->scene->player->position.x + 625, app->scene->player->position.y - 464);
	app->render->DrawTexture(currentTextureresta_2, app->scene->player->position.x + 419, app->scene->player->position.y - 309);
	app->render->DrawTexture(currentTexturesuma_2, app->scene->player->position.x + 625, app->scene->player->position.y - 309);

	return ret;
}

bool SceneSettings::OnGuiMouseClickEvent(GuiControl* control) {

	return true;
}

// Called before quitting
bool SceneSettings::CleanUp()
{
	LOG("Freeing best logo ever scene");

	SDL_DestroyTexture(Sound_1);
	SDL_DestroyTexture(Sound_2);
	SDL_DestroyTexture(Sound_3);
	SDL_DestroyTexture(Sound_4);
	SDL_DestroyTexture(Resta_1);
	SDL_DestroyTexture(Resta_2);
	SDL_DestroyTexture(Resta_3);
	SDL_DestroyTexture(Suma_1);
	SDL_DestroyTexture(Suma_2);
	SDL_DestroyTexture(Suma_3);

	app->guiManager->DestroyGuiControl(btn1);
	app->guiManager->DestroyGuiControl(btn2);
	app->guiManager->DestroyGuiControl(btn3);
	app->guiManager->DestroyGuiControl(btn4);
	app->guiManager->DestroyGuiControl(btn5);
	app->guiManager->DestroyGuiControl(btn6);

	return true;
}
