//------------------------------------------------------------------------
// GameTest.cpp
// ALPACA ESCAPE
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>

static float GenerateRandNum()
{	
	// Generate a random number from a uniform distribution.
	// We need a random number generator to spawn background objects i.e. zookeepers and waterbottles with a set probability.
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	return distribution(gen);
}


//------------------------------------------------------------------------
// Class Declarations
//------------------------------------------------------------------------
class Alpaca {
public:
	const float ALPACA_SPEED = 0.5f; // alpaca move speed
	const float SPIT_SPEED = 0.7f; // alpaca spit speed
	int hearts = 3; // default heart count
	int spitsCount = 5; // default spit count
	bool isInvincible = false;
	std::chrono::steady_clock::time_point invisibleStartTime; // we need a timer to keep track of alpaca invincibility after hitting a zookeeper
	CSimpleSprite* alpacaSprite;
	std::vector<CSimpleSprite*> spits;

	void AddSpit()
	{	
		// alpaca is spitting!!
		CSimpleSprite* spit;
		spit = App::CreateSprite(".\\TestData\\spit.png", 1, 1);

		float pos_x, pos_y;
		alpacaSprite->GetPosition(pos_x, pos_y);
		spit->SetPosition(pos_x + 35, pos_y + 25); // set it to be near the mouth coords of the alpaca
		spits.push_back(spit);
	}
};


class BackgroundObj {
// parent class of all background objects, makes it easier to extend the game by adding more different background objects
public:
	const float BACKGROUND_SPEED = 0.3f; // the speed at which all background objects are moving 
	std::vector<CSimpleSprite*> backgroundSprites;
	std::string imgName; // name of the image used to create the sprite

	void AddBackgroundObj()
	{	
		// spawn a background object
		CSimpleSprite* backgroundSprite;
		std::string parentDir = ".\\TestData\\";
		backgroundSprite = App::CreateSprite((parentDir + imgName).c_str(), 1, 1);

		// we want to make sure the objects stay within the screen, so we need to do some coords clamping
		auto clamp = [](float n, float min, float max) {
			if (n < min) n = min;
			if (n > max) n = max;
			return n;
			};

		// coords we get are the centre of a sprite, so subtract by half height
		float min = backgroundSprite->GetHeight() / 2.0f;
		float max = APP_VIRTUAL_HEIGHT - min;
		float pos_y = clamp(GenerateRandNum() * APP_VIRTUAL_HEIGHT, min, max);

		backgroundSprite->SetPosition(APP_VIRTUAL_WIDTH, pos_y);
		backgroundSprites.push_back(backgroundSprite);
	}

	void RemoveOutOfBoundObjs()
	{	
		// remove background objects that have moved out of screen so our vectors don't grow indefinitely
		auto isOutOfBound = [](CSimpleSprite* backgroundObj)
			{
				float posX, posY;
				backgroundObj->GetPosition(posX, posY);
				return posX + backgroundObj->GetWidth() / 2.0f < 0.0f;
			};

		// use remove_if since we're removing while looping
		backgroundSprites.erase(remove_if(backgroundSprites.begin(), backgroundSprites.end(), isOutOfBound), backgroundSprites.end());
	}
};


class Zookeepers : public BackgroundObj {
public:
	const int MAX_ZOOKEEPER_COUNT = 11; // the max number of zookeepers allowed on the screen at once, can be adjusted based on difficulty desired
	const float ZOOKEEPER_SPAWN_PROB = 0.05f; // the probablity to spawn a new zookeeper in any given frame

	Zookeepers(const std::string& imgName) : BackgroundObj() {
		this->imgName = imgName;
	}
};


class WaterBottles : public BackgroundObj {
public:
	const int MAX_WATERBOTTLE_COUNT = 2; // the max number of waterbottles allowed on the screen at once
	const float WATERBOTTLE_SPAWN_PROB = 0.008f; // the probablity to spawn a new waterbottle in any given frame

	WaterBottles(const std::string& imgName) : BackgroundObj() {
		this->imgName = imgName;
	}
};


// initialize all game objects
Alpaca* alpaca = new Alpaca;
Zookeepers* zookeepers = new Zookeepers("zookeeper.png");
WaterBottles* waterbottles = new WaterBottles("water.png");


//------------------------------------------------------------------------
// Utility Functions
//------------------------------------------------------------------------
static bool isColliding(CSimpleSprite* obj1, CSimpleSprite* obj2) {
	// check collition between obj1 and obj2

	float obj1Width = obj1->GetWidth() / 2.0f;
	float obj1Height = obj1->GetHeight() / 2.0f;
	float obj1X, obj1Y;
	obj1->GetPosition(obj1X, obj1Y);

	float obj2Width = obj2->GetWidth() / 2.0f;
	float obj2Height = obj2->GetHeight() / 2.0f;
	float obj2X, obj2Y;
	obj2->GetPosition(obj2X, obj2Y);

	// there is a collision if there is an intersection in both the x and y axes
	bool xCollision = std::abs(obj1X - obj2X) <= (obj1Width + obj2Width);
	bool yCollision = std::abs(obj1Y - obj2Y) <= (obj1Height + obj2Height);
	return (xCollision && yCollision);
}


static void HandleSpitZookeepersCollision()
{	
	std::vector<CSimpleSprite*> spitsToRemove, zookeepersToRemove;

	for (const auto& spit : alpaca->spits) {
		for (const auto& zookeeper : zookeepers->backgroundSprites) {
			// in case of a collision between a spit and a zookeeper, we want to remove both objects from screen
			if (isColliding(spit, zookeeper)) {
				spitsToRemove.push_back(spit);
				zookeepersToRemove.push_back(zookeeper);
			}
		}
	}

	for (const auto& spitToRemove : spitsToRemove) {
		alpaca->spits.erase(std::remove(alpaca->spits.begin(), alpaca->spits.end(), spitToRemove), alpaca->spits.end());
	}

	for (const auto& zookeeperToRemove : zookeepersToRemove) {
		zookeepers->backgroundSprites.erase(std::remove(zookeepers->backgroundSprites.begin(), zookeepers->backgroundSprites.end(), zookeeperToRemove), zookeepers->backgroundSprites.end());
	}
}


static void HandleAlpacaZookeeperCollision()
{
	for (const auto& zookeeper : zookeepers->backgroundSprites) {
		if (isColliding(alpaca->alpacaSprite, zookeeper)) {
			// if alpaca runs into a zookeeper, we rotate the zookeeper to make it look like they're run over by the alpaca, lol
			zookeeper->SetAngle(-PI/2.2f);
			if (!alpaca->isInvincible)
			{	
				// if alpaca is not invincible when it runs into a zookeeper, we decrese heart count and start the invincibility 
				App::PlaySound(".\\TestData\\Test.wav");
				alpaca->hearts--;
				alpaca->isInvincible = true;
				alpaca->invisibleStartTime = std::chrono::steady_clock::now();
				// the alpaca is set to be bigger and red and rotated to make it look like it's mad
				alpaca->alpacaSprite->SetColor(1.0f, 0.0f, 0.0f);
				alpaca->alpacaSprite->SetAngle(-0.35);
				alpaca->alpacaSprite->SetScale(1.2f);
			}
		}
	}
}


static void HandleAlpacaWaterBottleCollision()
{	
	std::vector<CSimpleSprite*> WaterBottlesToRemove;

	for (const auto& waterbottle : waterbottles->backgroundSprites) {
		if (isColliding(waterbottle, alpaca->alpacaSprite)) {
			// when alpaca picks up a waterbottle, we restore one spit count for the alpaca if the alpaca has spat since it's now hydrated!
			WaterBottlesToRemove.push_back(waterbottle);
			if (alpaca->spitsCount < 5) alpaca->spitsCount++; 
		}
	}

	for (const auto& WaterBottleToRemove : WaterBottlesToRemove) {
		// remove the waterbottles that were picked up by the alpaca
		waterbottles->backgroundSprites.erase(std::remove(waterbottles->backgroundSprites.begin(), waterbottles->backgroundSprites.end(), WaterBottleToRemove), waterbottles->backgroundSprites.end());
	}
}


static void UpdateAlpacaPos(float deltaTime)
{	
	// update the position of the alpaca when WASD keys are pressed
	float x, y;
	alpaca->alpacaSprite->GetPosition(x, y);
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		/*alpaca->SetAnimation(ANIM_RIGHT);*/
		x += alpaca->ALPACA_SPEED * deltaTime;
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		/*alpaca->SetAnimation(ANIM_LEFT);*/
		x -= alpaca->ALPACA_SPEED * deltaTime;
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		/*alpaca->SetAnimation(ANIM_FORWARDS);*/
		y += alpaca->ALPACA_SPEED * deltaTime;
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		/*alpaca->SetAnimation(ANIM_BACKWARDS);*/
		y -= alpaca->ALPACA_SPEED * deltaTime;
	}

	alpaca->alpacaSprite->SetPosition(x, y);
}


static void UpdateSpitPos(float deltaTime) {
	// update the spit position on screen. We move the spits to the left to make it look like the alpaca is running to the right
	for (const auto& spit : alpaca->spits)
	{
		float posX, posY;
		spit->GetPosition(posX, posY);
		posX += alpaca->SPIT_SPEED * deltaTime;
		spit->SetPosition(posX, posY);
	}
}


static void UpdateZookeeperPos(float deltaTime) {
	// update the zookeepers' positions on screen.
	for (const auto& zookeeper : zookeepers->backgroundSprites)
	{
		float posX, posY;
		zookeeper->GetPosition(posX, posY);
		posX -= zookeepers->BACKGROUND_SPEED * deltaTime;
		zookeeper->SetPosition(posX, posY);
	}
}


static void UpdateWaterBottlePos(float deltaTime) {
	// update the waterbottles' positions on screen.
	for (const auto& waterbottle : waterbottles->backgroundSprites)
	{
		float posX, posY;
		waterbottle->GetPosition(posX, posY);
		posX -= waterbottles->BACKGROUND_SPEED * deltaTime;
		waterbottle->SetPosition(posX, posY);
	}
}


static void DrawSpitsCount()
{
	// draw current spit count onto the screen
	for (int i = 0; i < alpaca->spitsCount; i++)
	{
		float posY = 700;
		float posX = 50 + 35 * i; // so that the spits don't overlap
		CSimpleSprite* spit = App::CreateSprite(".\\TestData\\spit.png", 1, 1);
		spit->SetScale(1.5f);
		spit->SetAngle(PI / 2.0f); 
		spit->SetPosition(posX, posY);
		spit->Draw();
	}
}


static void DrawHeartCount()
{	
	// draw current heart count onto the screen
	for (int i = 0; i < alpaca->hearts; i++)
	{
		float posY = 630;
		float posX = 60 + 55 * i; // so that the hearts don't overlap
		CSimpleSprite* heart = App::CreateSprite(".\\TestData\\heart.png", 1, 1);
		heart->SetScale(1.5f);
		heart->SetPosition(posX, posY);
		heart->Draw();
	}
}

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	alpaca->alpacaSprite = App::CreateSprite(".\\TestData\\alpaca.png", 1, 1);
	alpaca->alpacaSprite->SetPosition(100.0f, 400.0f);
}


//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{	
	if (alpaca->hearts) // only update everything on the screen when the game is still going i.e. when alpaca still got hearts left
	{
		UpdateAlpacaPos(deltaTime);
		// use the right key to control spitting of the alpaca
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, true))
		{
			if (alpaca->spitsCount)
			{
				alpaca->AddSpit();
				alpaca->spitsCount--;
				App::PlaySound(".\\TestData\\Test.wav");
			}
		}
		// if alpaca is current invincible, we wanna check if it's exceeded the invincibility time limit which i set to be 2 seconds
		// if it has been over 2 seconds, remove alpaca invincibility and restore its size, color, and angle to show that it's no longer super mad 
		if (alpaca->isInvincible)
		{
			if (std::chrono::steady_clock::now() >= alpaca->invisibleStartTime + std::chrono::seconds(2)) {
				alpaca->isInvincible = false;
				alpaca->alpacaSprite->SetColor(1.0f, 1.0f, 1.0f);
				alpaca->alpacaSprite->SetAngle(0.0f);
				alpaca->alpacaSprite->SetScale(1.0f);
			}
		}

		// the only object that is gauranteed to be present in-game at any given time is the alpaca
		// therefore we want to check if the other objects are present before performing actions on them
		bool hasSpits = !alpaca->spits.empty();
		bool hasZookeepers = !zookeepers->backgroundSprites.empty();
		bool hasWaterBottles = !waterbottles->backgroundSprites.empty();

		if (hasSpits) UpdateSpitPos(deltaTime);

		// spawn a new waterbottle/zookeeper based on a probability
		// also we want to make sure that the current object count has not exceeded the max count allowed so the screen doesn't get spammed and the game becomes impossible to play
		if (GenerateRandNum() < zookeepers->ZOOKEEPER_SPAWN_PROB && zookeepers->backgroundSprites.size() < zookeepers->MAX_ZOOKEEPER_COUNT) zookeepers->AddBackgroundObj();

		if (GenerateRandNum() < waterbottles->WATERBOTTLE_SPAWN_PROB && waterbottles->backgroundSprites.size() < waterbottles->MAX_WATERBOTTLE_COUNT) waterbottles->AddBackgroundObj();

		if (hasZookeepers)
		{
			zookeepers->RemoveOutOfBoundObjs();
			UpdateZookeeperPos(deltaTime);
			HandleAlpacaZookeeperCollision();
		}

		if (hasWaterBottles)
		{	
			waterbottles->RemoveOutOfBoundObjs();
			UpdateWaterBottlePos(deltaTime);
			HandleAlpacaWaterBottleCollision();
		}

		if (hasSpits && hasZookeepers) HandleSpitZookeepersCollision();
	}
}


//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	//------------------------------------------------------------------------
	// render everyone
	alpaca->alpacaSprite->Draw();

	if (!zookeepers->backgroundSprites.empty())
	{
		for (const auto& zookeeper : zookeepers->backgroundSprites)
			{
				zookeeper->Draw();
			}
	}

	if (!waterbottles->backgroundSprites.empty())
	{
		for (const auto& waterbottle : waterbottles->backgroundSprites)
		{
			waterbottle->Draw();
		}
	}
	
	if (!alpaca->spits.empty())
	{
		for (const auto& spit : alpaca->spits)
		{
			spit->Draw();
		}
	}

	// draw the stats on the screen 
	DrawSpitsCount();
	DrawHeartCount();
	//------------------------------------------------------------------------

	// says "game over" when game ends i.e. when alpaca runs out of hearts
	if (!alpaca->hearts)
	{
		App::Print(400, 400, "Game Over.", 1.0f, 0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}


//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	// make sure to free everything to prevent memory leak
	delete alpaca;
	delete zookeepers;
	delete waterbottles;
}