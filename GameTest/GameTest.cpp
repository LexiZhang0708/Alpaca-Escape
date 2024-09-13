#include "stdafx.h"
#include <windows.h> 
#include <math.h>  
#include "app/app.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <memory>
#include "Utils.h"
#include "Alpaca.h"
#include "BackgroundObj.h"
#include "CollisionHandler.h"


std::unique_ptr<Alpaca> alpaca;
std::unique_ptr<Zookeepers> zookeepers;
std::unique_ptr<WaterBottles> waterbottles;


// Game pause state
bool gamePaused = false;


void Init() {
    alpaca = std::make_unique<Alpaca>();
    zookeepers = std::make_unique<Zookeepers>("zookeeper.png");
    waterbottles = std::make_unique<WaterBottles>("water.png");
}


void Update(float deltaTime) {
    // Check if the pause button is pressed
    if (App::IsKeyPressed('P')) {
        gamePaused = !gamePaused; // Toggle the pause state
    }

    // If the game is paused, do not update game logic
    if (gamePaused) {
        return; // Skip game updates if paused
    }

    // Game is not paused, proceed with the usual update logic
    if (alpaca->hearts > 0) {
        alpaca->UpdateAlpacaPos(deltaTime);
        alpaca->UpdateSpitPos(deltaTime);

        // Check if the spitting button is pressed and alpaca still has spits left
        if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, true)) {
            if (alpaca->spitsCount > 0) {
                alpaca->AddSpit();  // Add a new spit
                alpaca->spitsCount--;  // Decrease spit count
                App::PlaySound(".\\TestData\\Test.wav");  // Play sound when spitting
            }
        }

        zookeepers->RemoveOutOfBoundObjs();
        waterbottles->RemoveOutOfBoundObjs();

        zookeepers->UpdateBackgroundObjPos(deltaTime, zookeepers->BACKGROUND_SPEED);
        waterbottles->UpdateBackgroundObjPos(deltaTime, waterbottles->BACKGROUND_SPEED);

        // Spawn a zookeeper if the random probability is met and the max count isn't exceeded
        if (GenerateRandNum() < zookeepers->ZOOKEEPER_SPAWN_PROB && zookeepers->GetBackgroundSprites().size() < zookeepers->MAX_ZOOKEEPER_COUNT) {
            zookeepers->AddBackgroundObj();
        }

        // Spawn a waterbottle if the random probability is met and the max count isn't exceeded
        if (GenerateRandNum() < waterbottles->WATERBOTTLE_SPAWN_PROB && waterbottles->GetBackgroundSprites().size() < waterbottles->MAX_WATERBOTTLE_COUNT) {
            waterbottles->AddBackgroundObj();
        }

        HandleAlpacaZookeeperCollision(alpaca, zookeepers);
        HandleAlpacaWaterBottleCollision(alpaca, waterbottles);
        HandleSpitZookeepersCollision(alpaca, zookeepers);

        // Check if the alpaca's invincibility has expired (e.g., 2 seconds)
        if (alpaca->isInvincible && (std::chrono::steady_clock::now() - alpaca->invisibleStartTime) > std::chrono::seconds(2)) {
            alpaca->ResetInvincibility();
        }
    }
}


void Render() {
    alpaca->Render();
    zookeepers->Render();
    waterbottles->Render();
    alpaca->DrawSpitsCount();
    alpaca->DrawHeartCount();
    // Draw all the spits
    for (const auto& spit : alpaca->GetSpits()) {
        spit->Draw();
    }

    // If the game is paused, display a "Paused" message
    if (gamePaused) {
        App::Print(400, 400, "Game Paused. Press Start to Resume.", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_TIMES_ROMAN_24);
    }
}


void Shutdown() {
 
}
