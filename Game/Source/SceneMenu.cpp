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

	Credit_1 = app->tex->Load("Assets/Screens/Credits_1.png");

	currentTexture = Menu_1;

	app->guiManager->Enable();
	app->scene->Disable();
	app->lastScreen->Disable();

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
bool SceneMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMenu::Update(float dt)
{
	if (credits)
	{
		currentTexture = Credit_1;
		btn6->state = GuiControlState::NORMAL;
	}
	if (!credits)
	{
		currentTexture = Menu_1;
		btn6->state = GuiControlState::DISABLED;
		if (app->scene->player->save == node.child("modes").attribute("save").as_bool("false"))
		{
			currentTexture = Menu_6;
			btn3->state = GuiControlState::DISABLED;
		}
		if (btn1->state == GuiControlState::FOCUSED)
		{
			currentTexture = Menu_11;
		}
		if (btn1->state == GuiControlState::PRESSED)
		{
			currentTexture = Menu_12;
		}
		if (btn2->state == GuiControlState::FOCUSED)
		{
			currentTexture = Menu_2;
		}
		if (btn2->state == GuiControlState::PRESSED)
		{
			currentTexture = Menu_3;
			app->fade->StartFadeToBlack(this, (Module*)app->scene, 0);
			app->scene->Enable();
		}
		if (app->scene->player->save == node.child("modes").attribute("save").as_bool("true"))
		{
			btn3->state = GuiControlState::NORMAL;
		}
		if (btn3->state == GuiControlState::FOCUSED)
		{
			currentTexture = Menu_4;
		}
		if (btn3->state == GuiControlState::PRESSED)
		{
			app->fade->StartFadeToBlack(this, (Module*)app->scene, 0);
			currentTexture = Menu_5;
			app->LoadRequest();
			app->scene->Enable();
		}
		if (btn4->state == GuiControlState::FOCUSED)
		{
			currentTexture = Menu_7;
		}
		if (btn4->state == GuiControlState::PRESSED)
		{
			currentTexture = Menu_8;
			/*app->fade->StartFadeToBlack(this, (Module*)app->sceneSettings, 0);*/
		}
		if (btn5->state == GuiControlState::FOCUSED)
		{
			currentTexture = Menu_9;
		}
		if (btn5->state == GuiControlState::PRESSED)
		{
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

	//app->render->DrawTexture(currentTexture, 30,380);

	app->render->DrawTexture(currentTexture, app->scene->player->position.x - 125, app->scene->player->position.y - 550);

	return ret;
}

bool SceneMenu::OnGuiMouseClickEvent(GuiControl* control) {

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

	//Hazme CleanUp de todo los que puedas
	app->tex->UnLoad(Menu_1);
	app->tex->UnLoad(Menu_2);
	app->tex->UnLoad(Menu_3);
	app->tex->UnLoad(Menu_4);
	app->tex->UnLoad(Menu_5);
	app->tex->UnLoad(Menu_6);
	app->tex->UnLoad(Menu_7);
	app->tex->UnLoad(Menu_8);
	app->tex->UnLoad(Menu_9);
	app->tex->UnLoad(Menu_10);
	app->tex->UnLoad(Menu_11);
	app->tex->UnLoad(Menu_12);
	app->tex->UnLoad(Credit_1);

	//Destruye los botones
	app->guiManager->Disable();

	return true;
}
