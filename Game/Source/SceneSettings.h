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
	SDL_Texture* Sound_1;
	SDL_Texture* Sound_2;
	SDL_Texture* Sound_3;
	SDL_Texture* Sound_4;
	SDL_Texture* Resta_1;
	SDL_Texture* Resta_2;
	SDL_Texture* Resta_3;
	SDL_Texture* Suma_1;
	SDL_Texture* Suma_2;
	SDL_Texture* Suma_3;
	SDL_Texture* Menu_11;

	SDL_Texture* currentTexture;
	SDL_Texture* currentTexturesuma_1;
	SDL_Texture* currentTextureresta_1;
	SDL_Texture* currentTexturesuma_2;
	SDL_Texture* currentTextureresta_2;

	GuiControlButton* btn1;
	GuiControlButton* btn2;
	GuiControlButton* btn3;
	GuiControlButton* btn4;
	GuiControlButton* btn5;

	GuiControlButton* btn6;

	int count;

	bool fullscreen;
	bool vsync = false;

	// Screen music
	uint logo_theme;

	pugi::xml_node node;

};

#endif // __SCENESETTINGS_H__