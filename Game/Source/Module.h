#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class PhysBody;
class GuiControl;

class Module
{
public:

	App* Application;
	Module(App* parent, bool startEnabled) : Application(parent), active(startEnabled)
	{

	}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// L14: TODO 2: Create new virtual methods to LoadState / SaveState
	virtual bool LoadState(pugi::xml_node node) {
		return true;
	}
	virtual bool SaveState(pugi::xml_node node) {
		return true;
	}

	virtual void Enable() {
		if (!active) {
			active = true;
			Start();
		}
	}

	virtual void Disable() {
		if (active) {
			active = false;
			CleanUp();
		}
	}

	inline bool isEnabled() {
		return active;
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}


public:

	SString name;
	bool active;

};

#endif // __MODULE_H__