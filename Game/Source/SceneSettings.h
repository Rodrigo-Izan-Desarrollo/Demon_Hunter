#ifndef __SCENESETTINGS_H__
#define __SCENESETTINGS_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;

class SceneSettings : public Module
{
public:

	SceneSettings(App* application, bool start_enabled = true);

	// Destructor
	virtual ~SceneSettings();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// GUI
	bool OnGuiMouseClickEvent(GuiControl* control);

	bool newgame = false;

private:

	// Texture
	//Menu_inicio
	SDL_Texture* Menu_1;
	SDL_Texture* Menu_2;
	SDL_Texture* Menu_3;
	SDL_Texture* Menu_4;
	SDL_Texture* Menu_5;
	SDL_Texture* Menu_6;
	SDL_Texture* Menu_7;
	SDL_Texture* Menu_8;
	SDL_Texture* Menu_9;
	SDL_Texture* Menu_10;
	SDL_Texture* Menu_11;
	SDL_Texture* Menu_12;

	SDL_Texture* Credit_1;

	SDL_Texture* currentTexture;

	GuiControlButton* btn1;
	GuiControlButton* btn2;
	GuiControlButton* btn3;
	GuiControlButton* btn4;
	GuiControlButton* btn5;

	GuiControlButton* btn6;

	int count;

	bool credits = false;

	// Screen music
	uint logo_theme;

	pugi::xml_node node;

};

#endif // __SCENESETTINGS_H__