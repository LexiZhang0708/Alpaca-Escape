// GameTest.cpp
#include "Alpaca.h"
#include "Zookeepers.h"
#include "WaterBottles.h"
#include <memory>

std::unique_ptr<Alpaca> alpaca;
std::unique_ptr<Zookeepers> zookeepers;
std::unique_ptr<WaterBottles> waterbottles;

void Init() {
    alpaca = std::make_unique<Alpaca>();
    zookeepers = std::make_unique<Zookeepers>();
    waterbottles = std::make_unique<WaterBottles>();
}

void Update(float deltaTime) {
    if (alpaca->GetHearts() > 0) {
        alpaca->UpdatePosition(deltaTime);
        alpaca->UpdateSpitPositions(deltaTime);
        zookeepers->UpdatePosition(deltaTime);
        waterbottles->UpdatePosition(deltaTime);
        alpaca->UpdateInvincibility();
    }
}

void Render() {
    alpaca->Draw();
    zookeepers->Draw();
    waterbottles->Draw();
    alpaca->DrawStats();

    if (alpaca->GetHearts() <= 0) {
        App::Print(400, 400, "Game Over.", 1.0f, 0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24);
    }
}

void Shutdown() {
    // Smart pointers will clean up automatically
}
