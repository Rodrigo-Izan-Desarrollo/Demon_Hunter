#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "SceneLogo.h"
#include "SceneMenu.h"
#include "SceneSettings.h"
#include "LastScreen.h"
#include "Scenepause.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	Timer timer = Timer();
	startupTime = Timer();
	frameTime = PerfTimer();
	lastSecFrameTime = PerfTimer();

	frames = 0;

	win = new Window(this);
	input = new Input(this);
	render = new Render(this);
	tex = new Textures(this);
	audio = new Audio(this);
	physics = new Physics(this);
	fade = new FadeToBlack(this, false);

	sceneLogo = new SceneLogo(this, true);
	sceneMenu = new SceneMenu(this, false);
	sceneSettings = new SceneSettings(this, false);
	scene = new Scene(this,false);
	scenePause = new ScenePause(this, false);
	lastScreen = new LastScreen(this, false);

	map = new Map(this, false);
	entityManager = new EntityManager(this, false);
	guiManager = new GuiManager(this, false);

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(physics);

	AddModule(lastScreen);
	AddModule(scenePause);
	AddModule(scene);
	AddModule(sceneSettings);
	AddModule(sceneMenu);
	AddModule(sceneLogo);

	AddModule(map);
	AddModule(entityManager);
	AddModule(guiManager);

	AddModule(fade);
	// Render last to swap buffer
	AddModule(render);



	LOG("Timer App Constructor: %f", timer.ReadMSec());
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	Timer timer = Timer();

	bool ret = LoadConfig();

	if(ret == true)
	{
		gameTitle = configNode.child("app").child("title").child_value();
		win->SetTitle(gameTitle.GetString());
		maxFrameDuration = configFile.child("config").child("app").child("maxFrameDuration").attribute("value").as_int();

		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			pugi::xml_node node = configNode.child(item->data->name.GetString());
			ret = item->data->Awake(node);
			item = item->next;
		}
	}

	LOG("Timer App Awake(): %f", timer.ReadMSec());

	return ret;
}

// Called before the first frame
bool App::Start()
{
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	LOG("Timer App Start(): %f", timer.ReadMSec());

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// This is a good place to call Load / Save functions
	double currentDt = frameTime.ReadMs();
	if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
		uint32 delay = (uint32) (maxFrameDuration - currentDt);

		PerfTimer delayTimer = PerfTimer();
		SDL_Delay(delay);
		//LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs());
	}

	// Amount of frames since startup
	frameCount++;

	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();
	
	// Amount of ms took the last update (dt)
	dt = (float) frameTime.ReadMs();

	// Amount of frames during the last second
	lastSecFrameCount++;

	// Average FPS for the whole game life
	if (lastSecFrameTime.ReadMs() > 1000) {
		lastSecFrameTime.Start();
		averageFps = (averageFps + lastSecFrameCount) / 2;
		framesPerSecond = lastSecFrameCount; 
		lastSecFrameCount = 0;
	}


	// Shows the time measurements in the window title
	static char title[256];
	if (vsync==true)
	{
		sprintf_s(title, 256, "%s: Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %I32u Frame Count: %I64u vsync: on ",
			gameTitle.GetString(), averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);
	}
	else
	{
		sprintf_s(title, 256, "%s: Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %I32u Frame Count: %I64u vsync: off ",
			gameTitle.GetString(), averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);
	}

	app->win->SetTitle(title);

	if (loadRequest) {
		loadRequest = false;
		LoadFromFile();
	}

	if (saveRequest) {
		saveRequest = false;
		SaveFromFile();
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		unlimited = !unlimited;
	}
	if (unlimited)
	{
		maxFrameDuration = 32;
	}
	else
	{
		maxFrameDuration = 16;
	}
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	LOG("Timer App CleanUp(): %f", timer.ReadMSec());

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return gameTitle.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Request a save data in an XML file 
bool App::LoadRequest() {

	bool ret = true;
	loadRequest = true;
	return ret;
}

// Request to load data from XML file 
bool App::SaveRequest() {
	bool ret = true;
	saveRequest = true;
	return true;
}

bool App::LoadFromFile() {

	bool ret = true;

	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

	if (result)
	{
		LOG("save_game.xml parsed without errors");

		// Iterates all modules and call the load of each with the part of the XML node that corresponds to the module
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(saveFile.child("game_state").child(item->data->name.GetString()));
			item = item->next;
		}

	}
	else
	{
		LOG("Error loading save_game.xml: %s", result.description());
		ret = false;
	}



	return ret;

}

bool App::SaveFromFile() {

	bool ret = true;

	pugi::xml_document saveFile;
	pugi::xml_node gameState = saveFile.append_child("game_state");

	// Iterates all modules and call the save of each with the part of the XML node that corresponds to the module
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		pugi::xml_node module = gameState.append_child(item->data->name.GetString());
		ret = item->data->SaveState(module);
		item = item->next;
	}

	ret = saveFile.save_file("save_game.xml");

	if (ret) LOG("Saved");
	else LOG("Error saving game state");

	return ret;

}

// L14:   TODO 5: Implement the method LoadFromFile() to actually load a xml file
// then call all the modules to load themselves

// L14:   TODO 7: Implement the xml save method SaveToFile() for current state
// check https://pugixml.org/docs/quickstart.html#modify


