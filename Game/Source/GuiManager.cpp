#include "GuiManager.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "GuiControlButton.h"
#include "Audio.h"

GuiManager::GuiManager(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	texture = app->tex->Load("Assets/hud/Lives.png");
	timer.Start();

	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(id, bounds, text);
		break;
	}

	//Set the observer
	guiControl->observer = observer;

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::DestroyGuiControl(GuiControl* entity)
{
	ListItem<GuiControl*>* item = guiControlsList.start;

	while (item != nullptr)
	{
		if (item->data == entity)
		{
			RELEASE(item->data);
			guiControlsList.Del(item);
			break;
		}
		item = item->next;
	}

	return true;
}

bool GuiManager::Update(float dt)
{	

	float elapsedTime = timer.ReadMSec();

	int minutes = static_cast<int>(elapsedTime / (1000 * 60));
	int seconds = static_cast<int>((elapsedTime / 1000) - (minutes * 60));

	char timeString[20];
	snprintf(timeString, sizeof(timeString), "%02d:%02d", minutes, seconds);

	app->render->DrawText(timeString, 900, 25, 100, 50);

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Update(dt);
		control = control->next;
	}

	app->render->DrawTexture(texture, 20, 20, NULL , SDL_FLIP_NONE , 0);
	
	char buffer[20];  // Suficientemente grande para almacenar el entero como cadena
	snprintf(buffer, sizeof(buffer), "%d", app->scene->player->lifes);

	const char* miVariable = buffer;

	app->render->DrawText(miVariable, 60, 15, 30, 50);

	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	//while (control != nullptr)
	//{
	//	RELEASE(control);
	//}

	return true;

	return false;
}



