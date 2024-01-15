//------------------------------------------------------------------------
// GameTest.cpp
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
	const float ALPACA_SPEED = 0.5f;
	const float SPIT_SPEED = 0.7f;
	int hearts = 3;
	int spitsCount = 5;
	bool isInvincible = false;
	std::chrono::steady_clock::time_point invisibleStartTime;
	CSimpleSprite* alpacaSprite;
	std::vector<CSimpleSprite*> spits;

	void AddSpit()
	{
		CSimpleSprite* spit;
		spit = App::CreateSprite(".\\TestData\\spit.png", 1, 1);

		float pos_x, pos_y;
		alpacaSprite->GetPosition(pos_x, pos_y);
		spit->SetPosition(pos_x + 35, pos_y + 25);
		spits.push_back(spit);
	}
};


class BackgroundObj {
public:
	const float BACKGROUND_SPEED = 0.3f;
	std::vector<CSimpleSprite*> backgroundSprites;
	std::string imgName;

	void AddBackgroundObj()
	{
		CSimpleSprite* backgroundSprite;
		std::string parentDir = ".\\TestData\\";
		backgroundSprite = App::CreateSprite((parentDir + imgName).c_str(), 1, 1);

		auto clamp = [](float n, float min, float max) {
			if (n < min) n = min;
			if (n > max) n = max;
			return n;
			};

		float min = backgroundSprite->GetHeight() / 2.0f;
		float max = APP_VIRTUAL_HEIGHT - min;
		float pos_y = clamp(GenerateRandNum() * APP_VIRTUAL_HEIGHT, min, max);

		backgroundSprite->SetPosition(APP_VIRTUAL_WIDTH, pos_y);
		backgroundSprites.push_back(backgroundSprite);
	}

	void RemoveOutOfBoundObjs()
	{
		auto isOutOfBound = [](CSimpleSprite* backgroundObj)
			{
				float posX, posY;
				backgroundObj->GetPosition(posX, posY);
				return posX + backgroundObj->GetWidth() / 2.0f < 0.0f;
			};

		backgroundSprites.erase(remove_if(backgroundSprites.begin(), backgroundSprites.end(), isOutOfBound), backgroundSprites.end());
	}
};


class Zookeepers : public BackgroundObj {
public:
	const int MAX_ZOOKEEPER_COUNT = 12;
	const float ZOOKEEPER_SPAWN_PROB = 0.05f;

	Zookeepers(const std::string& imgName) : BackgroundObj() {
		this->imgName = imgName;
	}
};


class WaterBottles : public BackgroundObj {
public:
	const int MAX_WATERBOTTLE_COUNT = 2;
	const float WATERBOTTLE_SPAWN_PROB = 0.008f;

	WaterBottles(const std::string& imgName) : BackgroundObj() {
		this->imgName = imgName;
	}
};


Alpaca* alpaca = new Alpaca;
Zookeepers* zookeepers = new Zookeepers("zookeeper.png");
WaterBottles* waterbottles = new WaterBottles("water.png");


//------------------------------------------------------------------------
// Utility Functions
//------------------------------------------------------------------------
static bool isColliding(CSimpleSprite* obj1, CSimpleSprite* obj2) {
	float obj1Width = obj1->GetWidth() / 2.0f;
	float obj1Height = obj1->GetHeight() / 2.0f;
	float obj1X, obj1Y;
	obj1->GetPosition(obj1X, obj1Y);

	float obj2Width = obj2->GetWidth() / 2.0f;
	float obj2Height = obj2->GetHeight() / 2.0f;
	float obj2X, obj2Y;
	obj2->GetPosition(obj2X, obj2Y);

	bool xCollision = std::abs(obj1X - obj2X) <= (obj1Width + obj2Width);
	bool yCollision = std::abs(obj1Y - obj2Y) <= (obj1Height + obj2Height);
	return (xCollision && yCollision);
}


static void HandleSpitZookeepersCollision()
{
	std::vector<CSimpleSprite*> spitsToRemove, zookeepersToRemove;

	for (const auto& spit : alpaca->spits) {
		for (const auto& zookeeper : zookeepers->backgroundSprites) {
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
			zookeeper->SetAngle(-PI/2.2f);
			if (!alpaca->isInvincible)
			{	
				App::PlaySound(".\\TestData\\Test.wav");
				alpaca->hearts--;
				alpaca->isInvincible = true;
				alpaca->invisibleStartTime = std::chrono::steady_clock::now();
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
			WaterBottlesToRemove.push_back(waterbottle);
			if (alpaca->spitsCount < 5) alpaca->spitsCount++; 
		}
	}

	for (const auto& WaterBottleToRemove : WaterBottlesToRemove) {
		waterbottles->backgroundSprites.erase(std::remove(waterbottles->backgroundSprites.begin(), waterbottles->backgroundSprites.end(), WaterBottleToRemove), waterbottles->backgroundSprites.end());
	}
}


static void UpdateAlpacaPos(float deltaTime)
{
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
	for (const auto& spit : alpaca->spits)
	{
		float posX, posY;
		spit->GetPosition(posX, posY);
		posX += alpaca->SPIT_SPEED * deltaTime;
		spit->SetPosition(posX, posY);
	}
}


static void UpdateZookeeperPos(float deltaTime) {
	for (const auto& zookeeper : zookeepers->backgroundSprites)
	{
		float posX, posY;
		zookeeper->GetPosition(posX, posY);
		posX -= zookeepers->BACKGROUND_SPEED * deltaTime;
		zookeeper->SetPosition(posX, posY);
	}
}


static void UpdateWaterBottlePos(float deltaTime) {
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
	for (int i = 0; i < alpaca->spitsCount; i++)
	{
		float posY = 700;
		float posX = 50 + 35 * i;
		CSimpleSprite* spit = App::CreateSprite(".\\TestData\\spit.png", 1, 1);
		spit->SetScale(1.5f);
		spit->SetAngle(PI / 2.0f);
		spit->SetPosition(posX, posY);
		spit->Draw();
	}
}


static void DrawHeartCount()
{
	for (int i = 0; i < alpaca->hearts; i++)
	{
		float posY = 630;
		float posX = 60 + 55 * i;
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
	if (alpaca->hearts)
	{
		UpdateAlpacaPos(deltaTime);
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, true))
		{
			if (alpaca->spitsCount)
			{
				alpaca->AddSpit();
				alpaca->spitsCount--;
				App::PlaySound(".\\TestData\\Test.wav");
			}
		}
		if (alpaca->isInvincible)
		{
			if (std::chrono::steady_clock::now() >= alpaca->invisibleStartTime + std::chrono::seconds(2)) {
				alpaca->isInvincible = false;
				alpaca->alpacaSprite->SetColor(1.0f, 1.0f, 1.0f);
				alpaca->alpacaSprite->SetAngle(0.0f);
				alpaca->alpacaSprite->SetScale(1.0f);
			}
		}

		bool hasSpits = !alpaca->spits.empty();
		bool hasZookeepers = !zookeepers->backgroundSprites.empty();
		bool hasWaterBottles = !waterbottles->backgroundSprites.empty();

		if (hasSpits) UpdateSpitPos(deltaTime);

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
	// Example Sprite Code....
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

	DrawSpitsCount();
	DrawHeartCount();
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	if (!alpaca->hearts)
	{
		App::Print(400, 400, "Game Over.", 1.0f, 0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	
	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	/*static float a = 0.0f;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	a += 0.1f;
	for (int i = 0; i < 20; i++)
	{

		float sx = 200 + sinf(a + i * 0.1f)*60.0f;
		float sy = 200 + cosf(a + i * 0.1f)*60.0f;
		float ex = 700 - sinf(a + i * 0.1f)*60.0f;
		float ey = 700 - cosf(a + i * 0.1f)*60.0f;
		g = (float)i / 20.0f;
		b = (float)i / 20.0f;
		App::DrawLine(sx, sy, ex, ey,r,g,b);
	}*/

}


//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	delete alpaca;
	delete zookeepers;
	delete waterbottles;
}