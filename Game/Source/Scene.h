#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Slime.h"
#include "Skeleton.h"
#include "Bomber.h"
#include "BigSlime.h"
#include "Boss.h"
#include "Powerup_1.h"
#include "Powerup_2.h"
#include "Powerup_3.h"
#include <list>
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

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
	
	//Entities
	
	//PLayer
	Player* player;

	//Enemies
	Slime* slime;
	Skeleton* skeleton;
	BigSlime* bigslime;
	Boss* boss;
	SlimeVolador* slimevolador;

	//Items
	Powerup_1* powerup_1;
	Powerup_2* powerup_2;
	Powerup_3* powerup_3;

	iPoint GetPLayerPosition();

	virtual bool LoadState(pugi::xml_node node);
	virtual bool SaveState(pugi::xml_node node);

	bool OnGuiMouseClickEvent(GuiControl* control);

private:

	SDL_Texture* img;

	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	const char* musicpathambient;
	uint ambient_Fx;

	pugi::xml_node configNode;

	List<Entity*> slimesList;
	List<Entity*> vslimesList;

	GuiControlButton* gcButtom;
};

#endif // __SCENE_H__