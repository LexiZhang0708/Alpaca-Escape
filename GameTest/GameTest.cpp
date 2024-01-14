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
//------------------------------------------------------------------------
// Example data....
//------------------------------------------------------------------------

struct Alpaca : CSimpleSprite {
	int hearts;
	int spitsCount;
};


struct Zookeepers : CSimpleSprite {};

const float ALPACA_SPEED = 0.5f;
const float ZOOKEEPER_SPEED = 0.3f;
const float SPIT_SPEED = 0.7f;
Alpaca *alpaca;
std::vector<CSimpleSprite *> zookeepers;
std::vector<CSimpleSprite *> spits;

//enum
//{
//	ANIM_FORWARDS,
//	ANIM_BACKWARDS,
//	ANIM_LEFT,
//	ANIM_RIGHT,
//};
//------------------------------------------------------------------------

static float GenerateRandNum()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	return distribution(gen);
}


static void AddZookeeper()
{	
	CSimpleSprite* zookeeper;
	zookeeper = App::CreateSprite(".\\TestData\\zookeeper2.png", 1, 1);

	auto clamp = [](float n, float min, float max) {
		if (n < min) n = min;
		if (n > max) n = max;
		return n;
		};

	float min = zookeeper->GetHeight() / 2.0f;
	float max = APP_VIRTUAL_HEIGHT - min;
	float pos_y = clamp(GenerateRandNum() * APP_VIRTUAL_HEIGHT, min, max);
	
	zookeeper->SetPosition(APP_VIRTUAL_WIDTH, pos_y);
	zookeepers.push_back(zookeeper);
}


static void AddSpit()
{
	CSimpleSprite* spit;
	spit = App::CreateSprite(".\\TestData\\spit.png", 1, 1);

	float pos_x, pos_y;
	alpaca->GetPosition(pos_x, pos_y);
	spit->SetPosition(pos_x+35, pos_y+25);
	spits.push_back(spit);
}


static void RemoveOutOfBoundZookepers()
{	
	auto isOutOfBound = [](CSimpleSprite* zookeeper)
	{	
		float posX, posY;
		zookeeper->GetPosition(posX, posY);
		return posX + zookeeper->GetWidth() / 2.0f < 0.0f; // Characters out of the screen
	};

	zookeepers.erase(remove_if(zookeepers.begin(), zookeepers.end(), isOutOfBound), zookeepers.end());
}


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
	std::vector<CSimpleSprite* > spitsToRemove, zookeepersToRemove;

	for (const auto& spit : spits) {
		for (const auto& zookeeper : zookeepers) {
			if (isColliding(spit, zookeeper)) {
				spitsToRemove.push_back(spit);
				zookeepersToRemove.push_back(zookeeper);
			}
		}
	}

	for (const auto& spitToRemove : spitsToRemove) {
		spits.erase(std::remove(spits.begin(), spits.end(), spitToRemove), spits.end());
	}

	for (const auto& zookeeperToRemove : zookeepersToRemove) {
		zookeepers.erase(std::remove(zookeepers.begin(), zookeepers.end(), zookeeperToRemove), zookeepers.end());
	}

}


//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	alpaca = static_cast<Alpaca* >(App::CreateSprite(".\\TestData\\alpaca2.png", 1, 1));
	alpaca->SetPosition(100.0f, 400.0f);
	//float speed = 1.0f / 15.0f;
	//testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	//testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	//testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	//testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	alpaca->Update(deltaTime);
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		/*alpaca->SetAnimation(ANIM_RIGHT);*/
		float x, y;
		alpaca->GetPosition(x, y);
		x += ALPACA_SPEED * deltaTime;
		alpaca->SetPosition(x, y);
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		/*alpaca->SetAnimation(ANIM_LEFT);*/
		float x, y;
		alpaca->GetPosition(x, y);
		x -= ALPACA_SPEED * deltaTime;
		alpaca->SetPosition(x, y);
	}
    if (App::GetController().GetLeftThumbStickY() > 0.5f)
    {
        /*alpaca->SetAnimation(ANIM_FORWARDS);*/
        float x, y;
        alpaca->GetPosition(x, y);
        y += ALPACA_SPEED * deltaTime;
        alpaca->SetPosition(x, y);
    }
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		/*alpaca->SetAnimation(ANIM_BACKWARDS);*/
		float x, y;
		alpaca->GetPosition(x, y);
		y -= ALPACA_SPEED * deltaTime;
		alpaca->SetPosition(x, y);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, true))
	{
		AddSpit();
		App::PlaySound(".\\TestData\\Test.wav");
	}

	for (const auto& spit : spits)
	{
		float posX, posY;
		spit->GetPosition(posX, posY);
		posX += SPIT_SPEED * deltaTime;
		spit->SetPosition(posX, posY);
	}

	HandleSpitZookeepersCollision();
		
	
	RemoveOutOfBoundZookepers();

	if (GenerateRandNum() < 0.05 && zookeepers.size() < 10)
	{
		AddZookeeper();
	}

	for (const auto& zookeeper : zookeepers)
	{
		float posX, posY;
		zookeeper->GetPosition(posX, posY);
		posX -= ZOOKEEPER_SPEED * deltaTime;
		zookeeper->SetPosition(posX, posY);
	}

	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	//{
	//	testSprite->SetScale(testSprite->GetScale() + 0.1f);
	//}
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	//{
	//	testSprite->SetScale(testSprite->GetScale() - 0.1f);
	//}
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	//{
	//	testSprite->SetAngle(testSprite->GetAngle() + 0.1f);
	//}
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	//{
	//	testSprite->SetAngle(testSprite->GetAngle() - 0.1f);
	//}
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	//{
	//	testSprite->SetAnimation(-1);
	//}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	alpaca->Draw();

	for (const auto& zookeeper : zookeepers)
	{
		zookeeper->Draw();
	}

	for (const auto& spit : spits)
	{
		spit->Draw();
	}
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	std::string size = std::to_string(zookeepers.size());
	const char* size_c = size.c_str();
	App::Print(100, 100, size_c);

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
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete alpaca;
	//------------------------------------------------------------------------
}
