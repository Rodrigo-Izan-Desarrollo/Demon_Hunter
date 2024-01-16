#ifndef __LASTSCREEN_H__
#define __LASTSCREEN_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;

class LastScreen : public Module
{
public:

	LastScreen(App* application, bool start_enabled = false);

	// Destructor
	virtual ~LastScreen();

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

private:

	// Texture
	//Menu_inicio
	SDL_Texture* Gameover;
	SDL_Texture* Missioncompleted;

	SDL_Texture* currentTexture;

	int count;

	// Screen music
	uint logo_theme;

};

#endif // __SCENELOGO_H__