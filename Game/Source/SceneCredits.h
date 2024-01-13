#ifndef __SCENECREDITS_H__
#define __SCENECREDITS_H__

#include "Module.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"

struct SDL_Texture;

class SceneCredits : public Module
{
public:

	SceneCredits(App* application, bool start_enabled = true);

	// Destructor
	virtual ~SceneCredits();

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
	SDL_Texture* Credits_1;
	SDL_Texture* Credits_2;
	SDL_Texture* currentTexture;

	GuiControlButton* btn1;

	int count;

	// Screen music
	uint logo_theme;

};

#endif // __SCENELOGO_H__