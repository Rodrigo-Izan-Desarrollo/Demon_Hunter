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
	texturePath_4_2 = parameters.attribute("texturepathgod_2").as_string();
	texturePathcheck = parameters.attribute("texturepathcheck").as_string();

	soundatack = parameters.attribute("soundatack").as_string();
	soundjump = parameters.attribute("soundjump").as_string();
	soundpickup = parameters.attribute("soundpathpickup").as_string();
	soundpowerup = parameters.attribute("soundpowerup").as_string();
	soundportal = parameters.attribute("soundportal").as_string();


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
	texture_4_2 = app->tex->Load(texturePath_4_2);
	texturecheck = app->tex->Load(texturePathcheck);

	//Initialize sound efects
	atack_Fx = app->audio->LoadFx(soundatack);
	jump_Fx = app->audio->LoadFx(soundjump);
	pick_up_Fx = app->audio->LoadFx(soundpickup);
	powerup_Fx = app->audio->LoadFx(soundpowerup);
	portal_Fx = app->audio->LoadFx(soundportal);

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
	player_attack_2.LoadAnimations("player_atack_2");
	player_dash.LoadAnimations("player_dash");
	checkpoint.LoadAnimations("checkpoint");


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

	if (position.x >= 10680 && position.x < 12250|| position.x >= 14420)
	{
		app->render->camera.x += 0;
	}
	else
	{
		app->render->camera.x = -(position.x - 125);
	}

	//Default animation
	if (!atacking && !jumping && inground && !dead && !Godmode && !sleeping && !dashing && !atacking_2)
	{
		if (currentAnimation != &player && !respawning)  // Asegúrate de que no estás cambiando la animación durante el respawn
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
	if (canpower_1 && canpower_2 && canpower_3)
	{
		canpower_4 = true;
	}
		//Activate individual powerup
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && canchange && canpower_1 && !dead)
	{
		app->audio->PlayFx(powerup_Fx);

		powerup_1 = !powerup_1;

		powerup_2 = false;
		powerup_3 = false;
		powerup_4 = false;

		canchange = false;
		powertempo = SDL_GetTicks();//Start power-up cooldown for changing powerups
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && canchange && canpower_2 && !dead)
	{
		app->audio->PlayFx(powerup_Fx);

		powerup_2 = !powerup_2;

		powerup_1 = false;
		powerup_3 = false;
		powerup_4 = false;

		canchange = false;
		powertempo = SDL_GetTicks();
	}

	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && canchange && canpower_3 && !dead)
	{
		app->audio->PlayFx(powerup_Fx);

		powerup_3 = !powerup_3;

		powerup_2 = false;
		powerup_1 = false;
		powerup_4 = false;

		canchange = false;
		powertempo = SDL_GetTicks();
	}

	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN && canchange && canpower_4 && !dead)
	{
		app->audio->PlayFx(powerup_Fx);

		powerup_4 = !powerup_4;

		powerup_3 = false;
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
		if (!check_1)
		{
			app->SaveRequest();
			check_1 = true;
			check_2 = false;
			check_3 = false;
			save = true;
		}
	}
	if (position.x >= 6781 && position.y == 994)
	{
		if (!check_2)
		{
			app->SaveRequest();
			check_1 = false;
			check_2 = true;
			check_3 = false;
			save = true;
		}
	}
	if (check_3)
	{
		app->SaveRequest();
		check_1 = false;
		check_2 = false;
		check_3 = true;
		save = true;
	}

	//Portals 
	if (portal)
	{	
		if (position.x > 3200  && position.x < 4100)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(4448), PIXEL_TO_METERS(990) }, 0);
		}

		if (position.x > 4500 && position.x < 4700)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(5792), PIXEL_TO_METERS(1024) }, 0);
		}

		if (position.x > 8500 && position.x < 8700)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(8416), PIXEL_TO_METERS(1346) }, 0);
		}

		if (position.x > 14300 && position.x < 14500)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(15232), PIXEL_TO_METERS(1024) }, 0);
			leftmode = true;
			rightmode = false;
		}

		if (position.x > 14700)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(14496), PIXEL_TO_METERS(1120) }, 0);
			rightmode = true;
			leftmode = false;
		}

		portal = false;
	}

	// Godmode
		//Activate Godmode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && !dead)
	{
		if (jumping)
		{
			jumping = false;
		}
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
		pbody->body->SetTransform({ PIXEL_TO_METERS(14000), PIXEL_TO_METERS(802) }, 0);
		//app->render->camera.x = -9535;
		//app->render->camera.y = -252;
	}

	//Movement inputs
		// For stop the player when is not moving
	if (app->input->GetKey(SDL_SCANCODE_A)==KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D)==KEY_IDLE || dead) 	{
		veljump.x = 0;
	}

		// Movement in x
	if (canmove && !Godmode && !dead && !dashing && app->scene->isEnabled())
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
			if (powerup_1 || powerup_4 && !jumping)//Faster with power-up-1
			{
				veljump.x = 4.5f;
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
			if (powerup_1 || powerup_4 && !jumping)
			{
				veljump.x = -4.5f;
			}
			else
			{
				veljump.x = -3;
			}
		}
	}

	//Respawn
		//Dead compobator
	if (dead)
	{
		canmove = false;
		currentAnimation = &player_dead;
		app->scene->pausa = false;
	}
		//Activation of respawn
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
		//Respawn funtion
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

		// Crea el pbody solo si aún no se ha creado o si el respawn es necesario
		if (!pbody->body)
		{
			// Crea el pbody
			pbody = app->physics->CreateCircle(position.x + 30, position.y + 30, 13, bodyType::DYNAMIC);
			pbody->listener = this;
			pbody->ctype = ColliderType::PLAYER;
		}


		// Configura la posición de respawn según el checkpoint actual
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

		jumping = false;
		inground = true;

		rightmode = true;
		leftmode = false;
		lifes--;

		canmove = true;
		respawning = false;
	}


	//Jump
		//Jump input
	if (!jumping && inground && !atacking && !dead && app->scene->isEnabled())
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			app->audio->PlayFx(jump_Fx);//Load sound efect
			jumping = true;
			inground = false;

			// Cambios en la lógica de salto
			if (powerup_1 || powerup_4)//With power-up-1 you jump higher 
			{
				veljump.y = -7;
			}
			else
			{
				veljump.y = -6;
			}

			// Evitar ajustes adicionales a la velocidad después del salto
			if (jumping)
			{
				currentAnimation = &player_jump;
			}
		}
	}
		//Finished animation
	if (currentAnimation == &player_jump && currentAnimation->HasFinished() && inground) { // Reiniciar el salto
		currentAnimation->Reset();//Reset Animation
		currentAnimation->loopCount = 0;
		jumping = false;
	}

	// Atack
		//Atack input
	if (!pbodyatack && (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT || app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) && !dead && !jumping && !invisible && canatack && app->scene->isEnabled())
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
		//Finished animation
	if (currentAnimation == &player_attack && currentAnimation->HasFinished()) {
		if (pbodyatack) {
			//Destroy pbodyatack
			pbodyatack->body->SetActive(false);
			app->physics->world->DestroyBody(pbodyatack->body);
			pbodyatack = nullptr;
		}

		atacking = false;
		canmove = true;

		// Reset Animation
		currentAnimation->Reset();
		currentAnimation->loopCount = 0;
	}

	if (SDL_GetTicks()-atacktempo >= 1000)
	{
		canatack = true;
	}

	// Atack_2
		//Atack_2 input
	if (!pbodyatack_2 && app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT && !dead && !jumping && !invisible && canatack_2 && app->scene->isEnabled())
	{
		if (powerup_4 || powerup_2)
		{
			app->audio->PlayFx(atack_Fx);//Load sound efect
			atacktempo_2 = SDL_GetTicks();// Start timer
			canatack_2 = false;
			atacking_2 = true;

			//Create de pbody for the atack
			if (rightmode)
			{
				pbodyatack_2 = app->physics->CreateRectangle(position.x + 50, position.y + 20, 37.5f, 5, bodyType::STATIC);
				pbodyatack_2->listener = this;
				pbodyatack_2->ctype = ColliderType::PATACK;
			}
			if (leftmode)
			{
				pbodyatack_2 = app->physics->CreateRectangle(position.x - 20, position.y + 20, 37.5f, 5, bodyType::STATIC);
				pbodyatack_2->listener = this;
				pbodyatack_2->ctype = ColliderType::PATACK;
			}

			if (atacking_2)
			{
				canmove = false;
				currentAnimation->Reset();//reset the animation
				currentAnimation = &player_attack_2;
				currentAnimation->loopCount = 0;
			}
		}
	}
		//Finished animation
	if (currentAnimation == &player_attack_2 && currentAnimation->HasFinished()) {
		if (pbodyatack_2) {
			//Destroy pbodyatack
			pbodyatack_2->body->SetActive(false);
			app->physics->world->DestroyBody(pbodyatack_2->body);
			pbodyatack_2 = nullptr;
		}

		atacking_2 = false;
		canmove = true;

		// Reset Animation
		currentAnimation->Reset();
		currentAnimation->loopCount = 0;
	}
		//Cooldown atack_2
	if (SDL_GetTicks()- atacktempo_2 >= 6000)
	{
		canatack_2 = true;
	}

	// Dash 
		//Hability input
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT && canmove && !dead && !jumping && candash)
	{
		if (powerup_4 || powerup_1)
		{
			dashing = true;
			candash = false;
			canmove = false;
			dashtempo = SDL_GetTicks();// Start cooldown
			if (dashing)
			{
				currentAnimation = &player_dash;

				if (rightmode)//Do the tp
				{
					pbody->body->SetTransform({ PIXEL_TO_METERS(position.x + 4), PIXEL_TO_METERS(position.y+0.5f) }, 0);
				}
				if (leftmode)
				{
					pbody->body->SetTransform({ PIXEL_TO_METERS(position.x - 4), PIXEL_TO_METERS(position.y+0.5f) }, 0);
				}
			}
		}
	}
		//Reset animation
	if (currentAnimation==&player_dash && currentAnimation->HasFinished())
	{
		currentAnimation->Reset();
		currentAnimation->loopCount = 0;
		canmove = true;
		dashing = false;
	}
		//Reset cooldown
	if (SDL_GetTicks()-dashtempo >= 6000)
	{
		candash = true;
	}

	//Invisible
		//Invisible input 	
	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && caninv)
	{
		invisible = !invisible;
		invtempo = SDL_GetTicks(); // Inicialize the timer
		caninv = false;
	}
		//Invisible timer
	if (invisible)
	{
		if (SDL_GetTicks() - invtempo >= 6000) // Desabilitate the hability
		{
			invisible = false;
			invtempo_2 = SDL_GetTicks(); // Inicialize cooldown
		}
	}
		//Invisible cooldown
	if (SDL_GetTicks() - invtempo_2 >= 6000) // Finished couldown
	{
		caninv = true;
	}

	//Sleep animation
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
	else if(powerup_4)
	{
		currentTexture = invisible ? texture_4_2 : texture_4;
	}
	else {
		currentTexture = texture;
	}

	// Determina la dirección de dibujo (izquierda o derecha) y ajusta la textura según sea necesario
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

	//Animation Update
	currentAnimation->Update();

	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::VIDA:
		lifes++;
		app->audio->PlayFx(pick_up_Fx);
		break;
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
		app->audio->PlayFx(portal_Fx);
		break;
	case ColliderType::ENEMY:
		inground = true;
		if (!Godmode && !pbodyatack && !pbodyatack_2) {
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
