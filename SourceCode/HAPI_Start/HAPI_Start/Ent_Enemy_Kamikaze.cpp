#include "Ent_Enemy_Kamikaze.h"
#include "Ent_Player.h"

void Ent_Enemy_Kamikaze::Update(float dt)
{
	Ent_Enemy::Update(dt);

	timer_suicide += dt;

	if (timer_suicide > k_timeUntilSuicide * LEVEL_SCALE)
	{
		if (!suicideState) KillMyself();
	}
}

void Ent_Enemy_Kamikaze::SetupEnemy(Vector2 position, Vector2 velocity, Vector2 acceleration, float fireDelay)
{
	Ent_Enemy::SetupEnemy(position, velocity, acceleration, fireDelay);
	//declaredDead = false;
	animator.SetAnimation(*vizPtr, "Enter");
	animator.shouldLoop = false;
	timer_suicide = 0;
	suicideState = false;

	if (audioManager != false) audioManager->PlaySound(eSFX::ekamiSpawn, true, 0.8f, RelativeScreenX());
}

void Ent_Enemy_Kamikaze::KillMyself()
{
	animator.shouldLoop = true;
	animator.SetAnimationFrame(0);
	animator.SetAnimation(*vizPtr, "Suicide");

	//animator.SetAnimationFrame(0);

	Ent_Player* nearestPlayer = FindClosestPlayer();

	if (nearestPlayer == nullptr)
	{
		//HAPI.UserMessage("Warning: Kamikaze enemy could not find any players.", "KAMIKAZE WARNING");
		std::cout << "Warning: Kamikaze enemy could not find any players." << std::endl;
		SetActive(false);
		return;
	}

	Vector2 targetPosition = nearestPlayer->GetPosition();
	Vector2 direction = (targetPosition - GetPosition());
	direction.NormaliseInPlace();

	velocity = direction * k_kamikazeSpeed;
	if (audioManager != false) audioManager->PlaySound(eSFX::ekamiLaunch, true, 0.8f, RelativeScreenX());

	suicideState = true;



}
