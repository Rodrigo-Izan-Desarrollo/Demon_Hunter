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

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepathnormal").as_string();
	texturePath_1 = parameters.attribute("texturepathatack").as_string();
	texturePath_2 = parameters.attribute("texturepathaspeed").as_string();
	texturePath_3 = parameters.attribute("texturepathainv").as_string();
	texturePath_4 = parameters.attribute("texturepathgod").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	texture_1 = app->tex->Load(texturePath_1);
	texture_2 = app->tex->Load(texturePath_2);
	texture_3 = app->tex->Load(texturePath_3);
	texture_4 = app->tex->Load(texturePath_4);

	pbody = app->physics->CreateCircle(position.x + 30, position.y + 30, 13, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

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

	//Invisible
	player_inv.PushBack({ 62, 1, 32, 32 });
	player_inv.PushBack({ 94, 1, 32, 32 });
	player_inv.PushBack({ 62, 1, 32, 32 });
	player_inv.PushBack({ 94, 1, 32, 32 });
	player_inv.PushBack({ 62, 1, 32, 32 });
	player_inv.PushBack({ 94, 1, 32, 32 });
	player_inv.PushBack({ 62, 1, 32, 32 });
	player_inv.PushBack({ 94, 1, 32, 32 });
	player_inv.PushBack({ 62, 1, 32, 32 });
	player_inv.PushBack({ 94, 1, 32, 32 });
	player_inv.PushBack({ 62, 1, 32, 32 });
	player_inv.PushBack({ 94, 1, 32, 32 });
	player_inv.speed = 0.075f;
	player_inv.loop = false;

	player_inv_sleep.PushBack({ 63, 34, 32, 32 });
	player_inv_sleep.PushBack({ 95, 34, 32, 32 });
	player_inv_sleep.speed = 0.2f;
	player_inv_sleep.loop = false;

	player_inv_run.PushBack({ 219, 98, 32, 32 });
	player_inv_run.PushBack({ 251, 98, 32, 32 });
	player_inv_run.PushBack({ 283, 98, 32, 32 });
	player_inv_run.PushBack({ 315, 98, 32, 32 });
	player_inv_run.PushBack({ 347, 98, 32, 32 });
	player_inv_run.PushBack({ 379, 98, 32, 32 });
	player_inv_run.PushBack({ 411, 98, 32, 32 });
	player_inv_run.PushBack({ 442, 98, 32, 32 });
	player_inv_run.PushBack({ 474, 98, 32, 32 });
	player_inv_run.speed = 0.2f;
	player_inv_run.loop = true;

	player_inv_jump.PushBack({ 219, 163, 32, 32 });
	player_inv_jump.PushBack({ 251, 163, 32, 32 });
	player_inv_jump.PushBack({ 283, 163, 32, 32 });
	player_inv_jump.PushBack({ 315, 163, 32, 32 });
	player_inv_jump.PushBack({ 347, 163, 32, 32 });
	player_inv_jump.PushBack({ 379, 163, 32, 32 });
	player_inv_jump.PushBack({ 411, 163, 32, 32 });
	player_inv_jump.PushBack({ 442, 163, 32, 32 });
	player_inv_jump.PushBack({ 474, 163, 32, 32 });
	player_inv_jump.speed = 0.2f;
	player_inv_jump.loop = false;

	currentAnimation = &player;

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 veljump = pbody->body->GetLinearVelocity();

	//Camara movement

	app->render->camera.x = -(position.x - 157);
	app->render->camera.y = -(position.y - 550);

	//Default animation

	if (!atacking && !jumping && inground && !dead && !Godmode && !sleeping)
	{
		if (currentAnimation != &player)
		{
			currentAnimation = &player;
			currentAnimation->loopCount = 0;
			currentAnimation->Reset();
		}
		if (invisible)
		{
			currentAnimation = &player_inv;
		}
	}

	//Power-ups

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		powerup_1 = !powerup_1;
		powerup_2 = false;
		powerup_3 = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		powerup_2 = !powerup_2;
		powerup_1 = false;
		powerup_3 = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		powerup_3 = !powerup_3;
		powerup_2 = false;
		powerup_1 = false;
	}

	//Checkpoints

	if (app->render->camera.x <= -2900)
	{
		check_1 = true;
		check_2 = false;
	}
	if (app->render->camera.x <= -5540)
	{
		check_2 = true;
		check_1 = false;
	}

	// Debug inputs

	// Godmode

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && !dead)
	{
		Godmode = !Godmode;
	}

	// Godmode movement
	if (Godmode && !dead) // Si el godmode esta activado 
	{
		speed = 0.5f;
		veljump = b2Vec2(0.0, -0.1675);//desabilitamos la gravedad
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {//Agregamos la posibilidad de moverse en todas las direcciones
			veljump.y = -5;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			veljump.y = 5 * dt;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			veljump.x = -speed * dt;
			app->render->camera.x = -(position.x - 157);
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			veljump.x = speed * dt;
			app->render->camera.x = -(position.x - 157);
		}
		currentAnimation = &player; // Hacemos que la animación no cambie
	}

	// TPs

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && !dead)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(-620 + 16), PIXEL_TO_METERS(950) }, 0);
		app->render->camera.x = 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && !dead)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(2600 + 16), PIXEL_TO_METERS(1080) }, 0);
		app->render->camera.x = -3225;
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && !dead)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(5990 + 16), PIXEL_TO_METERS(1010) }, 0);
		app->render->camera.x = -6610;
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
			if (powerup_2 && !jumping)//Cuando tenga activo el power-up 2 corre mas rapido
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
			if (powerup_2 && !jumping)
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

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && respawn>0 && dead) // Boton para repawnear cuando uno muere
	{
		currentAnimation->Reset();// Primero reseteamos la animacion de muerte ya que el loop= false
		if (!check_1 && !check_2)// Dependiendo del check cambia el repawn
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(-620 + 16), PIXEL_TO_METERS(950) }, 0);
			app->render->camera.x = 0;
		}
		if (check_1)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(2600 + 16), PIXEL_TO_METERS(1080) }, 0);
			app->render->camera.x = -3225;
		}
		if (check_2)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(5990 + 16), PIXEL_TO_METERS(1010) }, 0);
			app->render->camera.x = -6610;
		}

		canmove = true;// Una vez repawneado te puedes mover
		dead = false;
		rightmode = true;
		leftmode = false;
		respawn--;// Le restamos una vida
	}

	//Jump

	if (!jumping && inground && !dead) // If para pemitir saltar
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN )
		{
			jumping = true;
			inground = false;
			veljump.y = -6;
			pbody->body->SetLinearVelocity(veljump);
			if (jumping)
			{
				currentAnimation = &player_jump;
				currentAnimation->loopCount = 0;
			}
		}
	}

	//Atack

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT && !dead && !jumping || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && !dead && !jumping) //If para poder saltar
	{
		atacking = true;
		if (atacking)
		{
			canmove = false;
			currentAnimation->Reset();
			currentAnimation = &player_attack;
			currentAnimation->loopCount = 0;
		}
	}

	//Inv

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && powerup_3)
	{
		invisible = true;
	}

	//Finished animations

	//Lo que hacen estos if es reiniciar las aniamciones para que a pesar de que tengan el loop = false se vuelvan a generar des de el principio

	if (currentAnimation == &player_attack && currentAnimation->HasFinished()) { // Reiniciar el ataque
		atacking = false;
		canmove = true;
	}

	if (currentAnimation == &player_jump && currentAnimation->HasFinished() && inground) { // Reiniciar el salto
		currentAnimation->Reset();
		jumping = false;
	}

	//Reiniciar animación idle

	if (currentAnimation == &player && currentAnimation->HasFinished() && inground) {
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
			app->render->DrawTexture(texture_1, position.x, position.y, &currentAnimation->GetCurrentFrame());
		}
		if (leftmode == true)
		{
			app->render->DrawTexture(texture_1, position.x, position.y, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
		}
	}
	else if (powerup_2)
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
	else if (powerup_3)
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

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		inground = true; // Su esta en la colision de plataform activa que esta en el suelo
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;

	case ColliderType::ENEMY:
		if (!Godmode) // Si colisona con colision de enemigo activa dead y desactiva canmove
		{
			dead = true;
			canmove = false;
		}
		break;

	case ColliderType::WALL:
		LOG("Colission WALL");
		inground = false; // Si esta en la colision con wall desactiva que esta en el suelo
		break;
	}

}