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
	name.Create("Player");
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


	//Normal atack
	player_attack.PushBack({ 0,259, 32, 32 });
	player_attack.PushBack({ 31,259, 32, 32 });
	player_attack.PushBack({ 63,259, 32, 32 });
	player_attack.PushBack({ 95,259, 32, 32 });
	player_attack.PushBack({ 126,259, 32, 32 });
	player_attack.PushBack({ 163,259, 32, 32 });
	player_attack.PushBack({ 192,259, 32, 32 });
	player_attack.loop = false;
	player_attack.speed = 0.3f;

	currentAnimation = &player;

	return true;
}

bool Player::Update(float dt)
{

	b2Vec2 veljump = pbody->body->GetLinearVelocity();

	//Camara movement
	if (position.y < 550)
	{
		app->render->camera.y += 0;
	}
	else
	{
		app->render->camera.y = -(position.y - 550);
	}
	if (position.x > 9660)
	{
		app->render->camera.y += 0;
	}
	else
	{
		app->render->camera.x = -(position.x - 125);
	}

	//Default animation

	if (!atacking && !jumping && inground && !dead && !Godmode && !sleeping)
	{
		if (currentAnimation != &player)
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
		powerup_1 = !powerup_1;
		powerup_2 = false;
		powerup_3 = false;
		canchange = false;
		powertempo = SDL_GetTicks();//Start power-up cooldown for changing powerups
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && canchange && canpower_2 && !dead)
	{
		powerup_2 = !powerup_2;
		powerup_1 = false;
		powerup_3 = false;
		canchange = false;
		powertempo = SDL_GetTicks();
	}

	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && canchange && canpower_3 && !dead)
	{
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

	if (position.x == 3396 && position.y == 1058)
	{
		check_1 = true;
		check_2 = false;
		check_3 = false;
	}
	if (position.x == 6781 && position.y == 994)
	{
		check_2 = true;
		check_1 = false;
		check_3 = false;
	}
	if (position.x == 10430 && position.y == 802)
	{
		check_3 = true;
		check_2 = false;
		check_1 = false;
	}

	//Portals 
	if (portal)
	{
		// 182 32		
		pbody->body->SetTransform({ PIXEL_TO_METERS(2600 + 16), PIXEL_TO_METERS(1080) }, 0);
		portal = false;
	}
	// Godmode

	//Activate Godmode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && !dead)
	{
		Godmode = !Godmode;
	}

	// Godmode movement
	if (Godmode && !dead)//Allways tha Godmode is true and the player is not Dead
	{
		/*speed = 0.5f;*/
		veljump = b2Vec2(0.0, -0.1675);//Desabilited the GRAVITY_Y in player
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {//Now you can move in any direction
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
		currentAnimation = &player;
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
		pbody->body->SetTransform({ PIXEL_TO_METERS(10436), PIXEL_TO_METERS(802) }, 0);
		app->render->camera.x = -9535;
		app->render->camera.y = -252;
	}

	//Movement inputs

	// Si el player no esta pulsando los botones de moviemiento este no avanza

	if (app->input->GetKey(SDL_SCANCODE_A)==KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D)==KEY_IDLE) 	{
		veljump.x = 0;
	}

	// Movimiento de izquierda a derecha

	if (canmove && !Godmode && !dead)
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !atacking /*Para que no se solapen las animaciones */ ) // Moviemiento hacia la derecha 
		{
			rightmode = true;// El rightmode y el leftmode es para hacer flip a la imagen
			leftmode = false;
			sleeping = false;
			veljump.x = 3;
			if (inground && !jumping)// Esta condicion es para que la animcion de salto y correr no se solapen
			{
				currentAnimation = &player_speed;
			}
			if (powerup_1 && !jumping)//Cuando tenga activo el power-up 2 corre mas rapido
			{
				veljump.x = 4;
			}
			else // Sino corre normal
			{
				veljump.x = 3;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !atacking) // Lo mismo para la izquierda
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
		currentAnimation = &player_dead;
		currentAnimation->loopCount = 0;
	}

	if (SDL_GetTicks() - deadtempo >= 3000 && respawn > 0 && dead)
	{
		currentAnimation->Reset(); // Reinicia la animación de muerte aquí
		canmove = true;
		dead = false;
		rightmode = true;
		leftmode = false;
		respawn--;

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
	}

	//Jump

	if (!jumping && inground && !dead) // If para pemitir saltar
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN )
		{
			jumping = true;
			inground = false;
			if (powerup_1)
			{
				veljump.y = -7;
			}
			else
			{
				veljump.y = -6;
			}
			pbody->body->SetLinearVelocity(veljump);
			if (jumping)
			{
				currentAnimation = &player_jump;
				currentAnimation->loopCount = 0;
			}
		}
	}

	//Atack

	if (!pbodyatack && (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) && !dead && !jumping && !invisible && canatack)
	{
		atacktempo = SDL_GetTicks();  // Obtener el tiempo actual
		canatack = false;
		atacking = true;
		if (rightmode)
		{
			pbodyatack = app->physics->CreateRectangle(position.x + 33, position.y + 15, 10, 20, bodyType::STATIC);
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
			currentAnimation->Reset();
			currentAnimation = &player_attack;
			currentAnimation->loopCount = 0;
		}
	}

	//Invisible
	

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && caninv)
	{
		invisible = !invisible;
		invtempo = SDL_GetTicks(); // Inicializa el tiempo al presionar la tecla R
		caninv = false;
	}
	if (invisible)
	{
		if (SDL_GetTicks() - invtempo >= 6000) // Cuando el timepo es igual o mayor a 6000 ms se desactiva la habilidad
		{
			invisible = false;
			invtempo_2 = SDL_GetTicks(); // Inicializamos el couldown para poder volver a usar la habilidad
		}
	}
	if (SDL_GetTicks() - invtempo_2 >= 50000) // Habilitamos la habilidad despues de 10000 ms
	{
		caninv = true;
	}

	//Finished animations

	//Lo que hacen estos if es reiniciar las aniamciones para que a pesar de que tengan el loop = false se vuelvan a generar des de el principio

	// Atack animation


	if (currentAnimation == &player_attack && currentAnimation->HasFinished()) {
		if (pbodyatack) {
			pbodyatack->body->SetActive(false);
			app->physics->world->DestroyBody(pbodyatack->body);
			pbodyatack = nullptr;  // Establecer pbodyatack a nullptr después de destruir el cuerpo
		}

		// Verificar el cooldown antes de habilitar un nuevo ataque
		if (SDL_GetTicks() - atacktempo >= 300)
		{
			canatack = true;
		}

		atacking = false;
		canmove = true;

		// Restablecer la animación de ataque a su estado inicial
		currentAnimation->Reset();
		currentAnimation->loopCount = 0;
	}


	//Jump animation

	if (currentAnimation == &player_jump && currentAnimation->HasFinished() && inground) { // Reiniciar el salto
		currentAnimation->Reset();
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


	//Set the velocity of the pbody of the player

	pbody->body->SetLinearVelocity(veljump);

	//Update player position in pixels

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	currentAnimation->Update();

	// Dependiendo de si el player tiene un power-up o no cambia la textura con la que se trabaja

	// El right mode es para cuando la textura va hacia la derecha y el leftmode para cuando va hacia la izquierda 
	// Esto es posible gracias a la posibilidad de hacer un flip en el DrawTexture que es una implementación nuestra propia
	if (powerup_1)
	{
		if (rightmode == true) 
		{
			app->render->DrawTexture(texture_2, position.x, position.y, &currentAnimation->GetCurrentFrame());
		}
		if (leftmode == true)
		{
			app->render->DrawTexture(texture_2, position.x, position.y, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
		}
	}
	else if (powerup_2)
	{
		if (rightmode == true)
		{
			app->render->DrawTexture(texture_1, position.x, position.y, &currentAnimation->GetCurrentFrame());
		}
		if (leftmode == true)
		{
			app->render->DrawTexture(texture_1, position.x, position.y, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
		}
	}
	else if (powerup_3)
	{
		if (invisible)
		{
			if (rightmode == true)
			{
				app->render->DrawTexture(texture_3_2, position.x, position.y, &currentAnimation->GetCurrentFrame());
			}
			if (leftmode == true)
			{
				app->render->DrawTexture(texture_3_2, position.x, position.y, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
			}
		}
		else
		{
			if (rightmode == true)
			{
				app->render->DrawTexture(texture_3, position.x, position.y, &currentAnimation->GetCurrentFrame());
			}
			if (leftmode == true)
			{
				app->render->DrawTexture(texture_3, position.x, position.y, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
			}
		}
	}
	else
	{
		if (rightmode == true)
		{
			app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
		}
		if (leftmode == true)
		{
			app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
		}
	}

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
		break;
	case ColliderType::POWERUP_2:
		canpower_2 = true;
		break;
	case ColliderType::POWERUP_3:
		canpower_3 = true;
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
		if (!Godmode && !dead && !pbodyatack) {
			dead = true;
			canmove = false;
		}
		deadtempo = SDL_GetTicks();
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