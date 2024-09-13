#include <stdafx.h>
#include "CollisionHandler.h"
#include "Utils.h"
#include <iostream>

// Handle collision between spits and zookeepers
void HandleSpitZookeepersCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<Zookeepers>& zookeepers) {
    std::vector<CSimpleSprite*> spitsToRemove, zookeepersToRemove;

    // Loop through all spits and zookeepers
    for (const auto& spit : alpaca->GetSpits()) {
        for (const auto& zookeeper : zookeepers->GetBackgroundSprites()) {
            if (isColliding(spit.get(), zookeeper.get())) {
                spitsToRemove.push_back(spit.get());
                zookeepersToRemove.push_back(zookeeper.get());
            }
        }
    }

    // Remove the collided spits from alpaca's spits vector
    for (const auto& spitToRemove : spitsToRemove) {
        alpaca->RemoveSpit(spitToRemove);
    }

    // Remove the collided zookeepers from zookeepers' backgroundSprites vector
    for (const auto& zookeeperToRemove : zookeepersToRemove) {
        zookeepers->GetBackgroundSprites().erase(
            std::remove_if(zookeepers->GetBackgroundSprites().begin(), zookeepers->GetBackgroundSprites().end(),
                [zookeeperToRemove](const std::unique_ptr<CSimpleSprite>& zookeeper) {
                    return zookeeper.get() == zookeeperToRemove;
                }), zookeepers->GetBackgroundSprites().end());
    }
}

// Handle collision between alpaca and zookeepers
void HandleAlpacaZookeeperCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<Zookeepers>& zookeepers) {
    for (const auto& zookeeper : zookeepers->GetBackgroundSprites()) {
        if (isColliding(alpaca->GetSprite(), zookeeper.get())) {
            zookeeper->SetAngle(-PI / 2.2f);
            if (!alpaca->isInvincible) {
                App::PlaySound(".\\TestData\\Test.wav");
                alpaca->hearts--;
                alpaca->isInvincible = true;
                alpaca->invisibleStartTime = std::chrono::steady_clock::now();
                alpaca->SetAngryAppearance();
            }
        }
    }
}

// Handle collision between alpaca and waterbottles
void HandleAlpacaWaterBottleCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<WaterBottles>& waterbottles) {
    std::vector<CSimpleSprite*> waterBottlesToRemove;

    // Loop through all waterbottles and check for collision with the alpaca
    for (const auto& waterbottle : waterbottles->GetBackgroundSprites()) {
        if (isColliding(waterbottle.get(), alpaca->GetSprite())) {
            waterBottlesToRemove.push_back(waterbottle.get());
            if (alpaca->spitsCount < 5) {
                alpaca->spitsCount++;
            }
        }
    }

    // Remove the collided waterbottles from waterbottles' backgroundSprites vector
    for (const auto& waterBottleToRemove : waterBottlesToRemove) {
        waterbottles->GetBackgroundSprites().erase(
            std::remove_if(waterbottles->GetBackgroundSprites().begin(), waterbottles->GetBackgroundSprites().end(),
                [waterBottleToRemove](const std::unique_ptr<CSimpleSprite>& waterbottle) {
                    return waterbottle.get() == waterBottleToRemove;
                }), waterbottles->GetBackgroundSprites().end());
    }
}
