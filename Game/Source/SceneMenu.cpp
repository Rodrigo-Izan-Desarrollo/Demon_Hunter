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
	if (app->sceneMenu->isEnabled())
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

		Credit_1 = app->tex->Load("Assets/Screens/Credits_1.png");

		//Music
		app->audio->PlayMusic("Assets/Audio/Music/Menu.ogg", NULL, app->audio->volume);

		//Buttons
		btn1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Title", { 7, 30, 920, 290 }, this);
		btn2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Play", { 99, 339, 235, 90 }, this);
		btn3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Continue", { 92, 455, 254, 45 }, this);
		btn4 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Settings", { 112, 525, 213, 42 }, this);
		btn5 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Exit", { 0, 721, 51, 46 }, this);
		btn6 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Title", { 7, 30, 920, 290 }, this);

		//Hability to interact with the buttons
		btn1->state = GuiControlState::NORMAL;
		btn2->state = GuiControlState::NORMAL;
		btn4->state = GuiControlState::NORMAL;
		btn5->state = GuiControlState::NORMAL;
		btn6->state = GuiControlState::NORMAL;

		//Audio
		app->guiManager->Enable();

		//Current Texture
		currentTexture = Menu_1;
	}

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
	//If credits are active
	if (credits) {
		currentTexture = Credit_1;
		btn6->state = GuiControlState::NORMAL;
	}
	//If credits are not active
	else {
		//If the player has a save
		currentTexture = Menu_1;
		btn6->state = GuiControlState::DISABLED;

		//If the player has not a save
		if (app->scene->player->save == node.child("modes").attribute("save").as_bool("false")) {
			currentTexture = Menu_6;
			btn3->state = GuiControlState::DISABLED;
		}
		else if (btn1->state == GuiControlState::FOCUSED) {
			currentTexture = Menu_11;
		}
		else if (btn1->state == GuiControlState::PRESSED) {
			currentTexture = Menu_12;
		}

		else if (btn2->state == GuiControlState::FOCUSED) {
			currentTexture = Menu_2;
		}
		//If the player press the button go to the scene
		else if (btn2->state == GuiControlState::PRESSED) {
			currentTexture = Menu_3;

			app->fade->FadeToBlackScene(this, (Module*)app->scene, 60.0f);
			app->map->Enable();
			app->entityManager->Enable();
			app->scene->player->lifes = 2;
			newgame = true;

		}

		//If the player has a save active the button
		else if (app->scene->player->save == node.child("modes").attribute("save").as_bool("true")) {
			btn3->state = GuiControlState::NORMAL;
		}
		else if (btn3->state == GuiControlState::FOCUSED) {
			currentTexture = Menu_4;
		}
		//If the player has a save and press the button go to the scene
		else if (btn3->state == GuiControlState::PRESSED) {
			currentTexture = Menu_5;
			app->fade->FadeToBlackScene(this, (Module*)app->scene, 60.0f);
			app->map->Enable();
			app->entityManager->Enable();
			app->scene->Enable();
		}

		else if (btn4->state == GuiControlState::FOCUSED) {
			currentTexture = Menu_7;
		}
		//If the player press the button go to the scenesettings
		else if (btn4->state == GuiControlState::PRESSED) {
			currentTexture = Menu_8;
			app->fade->FadeToBlackScene(this, (Module*)app->sceneSettings, 60.0f);
		}

		else if (btn5->state == GuiControlState::FOCUSED) {
			currentTexture = Menu_9;
		}
		else if (btn5->state == GuiControlState::PRESSED) {
			currentTexture = Menu_10;
			return false;
		}
	}

	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	bool ret = true;

	// Draw everything --------------------------------------
	if (app->scene->player==nullptr)
	{
		app->render->DrawTexture(currentTexture, 30, 380);
	}
	else
	{
		app->render->DrawTexture(currentTexture, app->scene->player->position.x - 125, app->scene->player->position.y - 550);
	}

	return ret;
}

bool SceneMenu::OnGuiMouseClickEvent(GuiControl* control) {

	//Change the variable credits
	if (control == btn1 && !credits)
	{
		credits = true;
	}
	if (control == btn6 && credits)
	{
		credits = false;
	}
	return true;
}

// Called before quitting
bool SceneMenu::CleanUp()
{
	LOG("Freeing best logo ever scene");

	//Unload textures
	SDL_DestroyTexture(Menu_1);
	SDL_DestroyTexture(Menu_2);
	SDL_DestroyTexture(Menu_3);
	SDL_DestroyTexture(Menu_4);
	SDL_DestroyTexture(Menu_5);
	SDL_DestroyTexture(Menu_6);
	SDL_DestroyTexture(Menu_7);
	SDL_DestroyTexture(Menu_8);
	SDL_DestroyTexture(Menu_9);
	SDL_DestroyTexture(Menu_10);
	SDL_DestroyTexture(Menu_11);
	SDL_DestroyTexture(Menu_12);
	SDL_DestroyTexture(Credit_1);

	//Unload buttons
	app->guiManager->DestroyGuiControl(btn1);
	app->guiManager->DestroyGuiControl(btn2);
	app->guiManager->DestroyGuiControl(btn3);
	app->guiManager->DestroyGuiControl(btn4);
	app->guiManager->DestroyGuiControl(btn5);
	app->guiManager->DestroyGuiControl(btn6);

	//Unload music
	app->audio->UnloadMusic("Assets/Audio/Music/Menu.ogg");

	
	return true;
}
