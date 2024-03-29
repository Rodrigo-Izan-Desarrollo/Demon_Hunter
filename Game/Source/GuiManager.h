#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"
#include "Timer.h"
#include "List.h"

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager(App* application, bool start_enabled);

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 // Called each loop iteration
	 bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	//DestroyGuiControl
	bool DestroyGuiControl(GuiControl* entity);

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds = { 0,0,0,0 });



public:

	List<GuiControl*> guiControlsList;
	SDL_Texture* texture;

	Timer  timer;
	bool timerPaused = false;

};

#endif // __GUIMANAGER_H__
