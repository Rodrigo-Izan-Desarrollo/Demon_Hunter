#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");
}

Player::~Player() {

}

bool Player::Awake() {

	//Conexion of parameters with config.xml

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepathnormal").as_string();
	texturePath_1 = parameters.attribute("texturepathatack").as_string();
	texturePath_2 = parameters.attribute("texturepathaspeed").as_string();
	texturePath_3 = parameters.attribute("texturepathainv").as_string();
	texturePath_3_2 = parameters.attribute("texturepathainv_2").as_string();
	texturePath_4 = parameters.attribute("texturepathgod").as_string();
	texturePath_flash = parameters.attribute("texturepathflash").as_string();

	musicpathatack = parameters.attribute("musicpathatack").as_string();
	musicpathjump = parameters.attribute("musicpathjump").as_string();
	musicpathpickup = parameters.attribute("musicpathpickup").as_string();
	musicpathpowerup = parameters.attribute("musicpathpowerup").as_string();


	return true;
}

bool Player::Start() {

	//Initilize textures
	texture = app->tex->Load(texturePath);
	texture_1 = app->tex->Load(texturePath_1);
	texture_2 = app->tex->Load(texturePath_2);
	texture_3 = app->tex->Load(texturePath_3);
	texture_3_2 = app->tex->Load(texturePath_3_2);
	texture_4 = app->tex->Load(texturePath_4);
	texture_flash = app->tex->Load(texturePath_flash);

	//Initialize sound efects
	atack_Fx = app->audio->LoadFx(musicpathatack);
	jump_Fx = app->audio->LoadFx(musicpathjump);
	pick_up_Fx = app->audio->LoadFx(musicpathpickup);
	powerup_Fx = app->audio->LoadFx(musicpathpowerup);


	//Create de pbody
	pbody = app->physics->CreateCircle(position.x + 30, position.y + 30, 13, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	//Conexion of Animations with config.xml
	player.LoadAnimations("player");
	player_speed.LoadAnimations("player_speed");
	player_jump.LoadAnimations("player_jump");
	player_dead.LoadAnimations("player_dead");
	player_sleep.LoadAnimations("player_sleep");
	player_attack.LoadAnimations("player_atack");

	currentAnimation = &player;

	return true;
}

bool Player::Update(float dt)
{

	b2Vec2 veljump = pbody->body->GetLinearVelocity();

	//Camara movement
	if (position.y <= 550)
	{
		app->render->camera.y += 0;
	}
	else
	{
		app->render->camera.y = -(position.y - 550);
	}
	if (position.x >= 14500)
	{
		app->render->camera.x += 0;
	}
	else
	{
		app->render->camera.x = -(position.x - 125);

	}

	//Default animation

	if (!atacking && !jumping && inground && !dead && !Godmode && !sleeping)
	{
		if (currentAnimation != &player && !respawning)  // Aseg�rate de que no est�s cambiando la animaci�n durante el respawn
		{
			currentAnimation = &player;
			currentAnimation->loopCount = 0;
			currentAnimation->Reset();
		}
	}


	//Power-ups

	//Activate all Power-ups
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		canpower_1 = true;
		canpower_2 = true;
		canpower_3 = true;
	}

	//Activate individual powerup
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && canchange && canpower_1 && !dead)
	{
		app->audio->PlayFx(powerup_Fx);
		powerup_1 = !powerup_1;
		powerup_2 = false;
		powerup_3 = false;
		canchange = false;
		powertempo = SDL_GetTicks();//Start power-up cooldown for changing powerups
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && canchange && canpower_2 && !dead)
	{
		app->audio->PlayFx(powerup_Fx);
		powerup_2 = !powerup_2;
		powerup_1 = false;
		powerup_3 = false;
		canchange = false;
		powertempo = SDL_GetTicks();
	}

	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && canchange && canpower_3 && !dead)
	{
		app->audio->PlayFx(powerup_Fx);
		powerup_3 = !powerup_3;
		powerup_2 = false;
		powerup_1 = false;
		canchange = false;
		powertempo = SDL_GetTicks();
	}

	//Activate the possibility of changing power-ups
	if (SDL_GetTicks() - powertempo >= 15000)
	{
		canchange = true;
	}

	//Checkpoints

	if (position.x >= 3396 && position.y == 1058)
	{
		check_1 = true;
		check_2 = false;
		check_3 = false;
	}
	if (position.x >= 6781 && position.y == 994)
	{
		check_2 = true;
		check_1 = false;
		check_3 = false;
	}
	if (position.x >= 10430 && position.y == 802)
	{
		check_3 = true;
		check_2 = false;
		check_1 = false;
	}

	//Portals 
	if (portal)
	{	
		pbody->body->SetTransform({ PIXEL_TO_METERS(2600 + 16), PIXEL_TO_METERS(1080) }, 0);
		portal = false;
	}

	// Godmode

	//Activate Godmode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && !dead)
	{
		Godmode = !Godmode;
	}

	//Allways currentanimation = player
	if (Godmode && currentAnimation != &player)
	{
		currentAnimation = &player;
	}

	// Godmode movement
	if (Godmode && !dead)
	{
		/*speed = 0.5f;*/
		veljump = b2Vec2(0.0, -0.1675);//Desabilited the GRAVITY_Y in player
		//Permision to move in any direction
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			veljump.y = -5;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			veljump.y = 5;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			veljump.x = -8;
			leftmode = true;
			rightmode = false;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			veljump.x = 8;
			leftmode = false;
			rightmode = true;
		}
	}

	// TPs

	//You can be teleported to all checkpoints
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && !dead)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(-620 + 16), PIXEL_TO_METERS(950) }, 0);
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && !dead)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(2600 + 16), PIXEL_TO_METERS(1080) }, 0);
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && !dead)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(5990 + 16), PIXEL_TO_METERS(1010) }, 0);
	}
	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN && !dead)
	{
		//Is different because we want to stop the camera in a specific position
		pbody->body->SetTransform({ PIXEL_TO_METERS(10436), PIXEL_TO_METERS(802) }, 0);
		app->render->camera.x = -9535;
		app->render->camera.y = -252;
	}

	//Movement inputs

	// For stop the player when is not moving

	if (app->input->GetKey(SDL_SCANCODE_A)==KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D)==KEY_IDLE || dead) 	{
		veljump.x = 0;
	}

	// Movement in x

	if (canmove && !Godmode && !dead )
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !atacking /*Para que no se solapen las animaciones */ )  
		{
			rightmode = true;// El rightmode and leftmode for the fleep
			leftmode = false;
			sleeping = false;//For the sleeping animation
			veljump.x = 3;
			if (inground && !jumping)// Esta condicion es para que la animcion de salto y correr no se solapen
			{
				currentAnimation = &player_speed;
			}
			if (powerup_1 && !jumping)//Faster with power-up-1
			{
				veljump.x = 4;
			}
			else
			{
				veljump.x = 3;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !atacking) 
		{
			leftmode = true;
			rightmode = false;
			sleeping = false;
			speed = -speed;
			veljump.x = -3;
			if (inground && !jumping)
			{
				currentAnimation = &player_speed;
			}
			if (powerup_1 && !jumping)
			{
				veljump.x = -4;
			}
			else
			{
				veljump.x = -3;
			}
		}
	}

	//Respawn

	if (dead)
	{
		canmove = false;
		currentAnimation = &player_dead;
	}

	if (currentAnimation == &player_dead && currentAnimation->HasFinished() && lifes > 0 && dead && !respawning)
	{
		// Almacena el cuerpo antiguo antes de destruirlo
		b2Body* oldBody = pbody->body;

		// Desactiva el cuerpo antiguo
		if (oldBody)
		{
			oldBody->SetActive(false);
		}

		// Destruye el cuerpo antiguo
		app->physics->world->DestroyBody(oldBody);
		pbody->body = nullptr;  // Importante: asigna nullptr para evitar acceder a memoria liberada

		currentAnimation->Reset(); // Reset dead animation
		currentAnimation->loopCount = 0;

		respawning = true;
	}



	if (respawning)
	{
		dead = false;

		// Elimina el antiguo cuerpo del respawn anterior si existe
		if (pbody->body)
		{
			pbody->body->SetActive(false);
			app->physics->world->DestroyBody(pbody->body);
			pbody->body = nullptr;
		}

		// Crea el pbody solo si a�n no se ha creado o si el respawn es necesario
		if (!pbody->body)
		{
			// Crea el pbody
			pbody = app->physics->CreateCircle(position.x + 30, position.y + 30, 13, bodyType::DYNAMIC);
			pbody->listener = this;
			pbody->ctype = ColliderType::PLAYER;
		}


		// Configura la posici�n de respawn seg�n el checkpoint actual
		if (!check_1 && !check_2 && !check_3)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(-620 + 16), PIXEL_TO_METERS(950) }, 0);
			app->render->camera.x = 0;
		}
		else if (check_1)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(2600 + 16), PIXEL_TO_METERS(1080) }, 0);
			app->render->camera.x = -3225;
		}
		else if (check_2)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(5990 + 16), PIXEL_TO_METERS(1010) }, 0);
			app->render->camera.x = -6610;
		}
		else if (check_3)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(10436), PIXEL_TO_METERS(802) }, 0);
			app->render->camera.x = -9535;
		}

		jumping = false;
		inground = true;

		rightmode = true;
		leftmode = false;
		lifes--;

		canmove = true;
		respawning = false;
	}


	//Jump

	if (!jumping && inground && !atacking && !dead)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			app->audio->PlayFx(jump_Fx);//Load sound efect
			jumping = true;
			inground = false;

			// Cambios en la l�gica de salto
			if (powerup_1)//With power-up-1 you jump higher 
			{
				veljump.y = -7;
			}
			else
			{
				veljump.y = -6;
			}

			// Evitar ajustes adicionales a la velocidad despu�s del salto
			// pbody->body->SetLinearVelocity(veljump); // Comentado para evitar interferencias
			if (jumping)
			{
				currentAnimation = &player_jump;
			}
		}
	}


	//Atack

	if (!pbodyatack && (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) && !dead && !jumping && !invisible && canatack)
	{
		app->audio->PlayFx(atack_Fx);//Load sound efect
		atacktempo = SDL_GetTicks();// Start timer
		canatack = false;
		atacking = true;
		//Create de pbody for the atack
		if (rightmode)
		{
			pbodyatack = app->physics->CreateRectangle(position.x + 35, position.y + 15, 10, 20, bodyType::STATIC);
			pbodyatack->listener = this;
			pbodyatack->ctype = ColliderType::PATACK;
		}
		if (leftmode)
		{
			pbodyatack = app->physics->CreateRectangle(position.x-2, position.y + 15, 10, 20, bodyType::STATIC);
			pbodyatack->listener = this;
			pbodyatack->ctype = ColliderType::PATACK;
		}

		if (atacking)
		{
			canmove = false;
			currentAnimation->Reset();//reset the animation
			currentAnimation = &player_attack;
			currentAnimation->loopCount = 0;
		}
	}

	//Invisible
	

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && caninv)
	{
		invisible = !invisible;
		invtempo = SDL_GetTicks(); // Inicialize the timer
		caninv = false;
	}

	if (invisible)
	{
		if (SDL_GetTicks() - invtempo >= 6000) // Desabilitate the hability
		{
			invisible = false;
			invtempo_2 = SDL_GetTicks(); // Inicialize cooldown
		}
	}
	if (SDL_GetTicks() - invtempo_2 >= 6000) // Finished couldown
	{
		caninv = true;
	}

	//Finished animations


	// Atack animation

	if (currentAnimation == &player_attack && currentAnimation->HasFinished()) {
		if (pbodyatack) {
			//Destroy pbodyatack
			pbodyatack->body->SetActive(false);
			app->physics->world->DestroyBody(pbodyatack->body);
			pbodyatack = nullptr;  
		}

		//Atacktempo
		if (SDL_GetTicks() - atacktempo >= 300)
		{
			canatack = true;
		}

		atacking = false;
		canmove = true;

		// Reset Animation
		currentAnimation->Reset();
		currentAnimation->loopCount = 0;
	}


	//Jump animation

	if (currentAnimation == &player_jump && currentAnimation->HasFinished() && inground) { // Reiniciar el salto
		currentAnimation->Reset();//Reset Animation
		currentAnimation->loopCount = 0;
		jumping = false;
	}

	//Idle animation

	if (currentAnimation == &player && currentAnimation->HasFinished() && inground && !Godmode) {
		if (currentAnimation != &player_sleep)
		{
			sleeping = true;
			currentAnimation = &player_sleep;
			currentAnimation->loopCount = 0;
			currentAnimation->Reset();
		}
	}

	//Depens on the power up change the texture

	if (powerup_1) {
		currentTexture = texture_2;
	}
	else if (powerup_2) {
		currentTexture = texture_1;
	}
	else if (powerup_3) {
		currentTexture = invisible ? texture_3_2 : texture_3;
	}
	else {
		currentTexture = texture;
	}

	// Determina la direcci�n de dibujo (izquierda o derecha) y ajusta la textura seg�n sea necesario

	if (rightmode) {
		app->render->DrawTexture(currentTexture, position.x, position.y, &currentAnimation->GetCurrentFrame());
	}
	else if (leftmode) {
		app->render->DrawTexture(currentTexture, position.x, position.y, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	}

	//Set the velocity of the pbody of the player

	pbody->body->SetLinearVelocity(veljump);

	//Update player position in pixels

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	currentAnimation->Update();

	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {
	case ColliderType::POWERUP_1:
		canpower_1 = true;
		app->audio->PlayFx(pick_up_Fx);
		break;
	case ColliderType::POWERUP_2:
		canpower_2 = true;
		app->audio->PlayFx(pick_up_Fx);
		break;
	case ColliderType::POWERUP_3:
		canpower_3 = true;
		app->audio->PlayFx(pick_up_Fx);
		break;
	case ColliderType::PLATFORM:
		inground = true;
		break;
	case ColliderType::WALLE:
		inground = true;
		break;
	case ColliderType::PORTAL:
		portal = true;
		break;
	case ColliderType::ENEMY:
		inground = true;
		if (!Godmode && !pbodyatack) {
			dead = true;
		}
		break;
	case ColliderType::WALL:
		inground = false;
		break;
	case ColliderType::PATACK:
		break;
	default:
		break;
	}
}