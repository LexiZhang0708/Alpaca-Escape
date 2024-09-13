#include <stdafx.h>
#include "CollisionHandler.h"
#include "Utils.h"
#include <iostream>

//------------------------------------------------------------------------
// Handle collision between spits and zookeepers
// Detects collisions between alpaca's spits and zookeepers, and removes
// the respective spit and zookeeper sprites upon collision.
//------------------------------------------------------------------------
void HandleSpitZookeepersCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<Zookeepers>& zookeepers) {
    std::vector<CSimpleSprite*> spitsToRemove, zookeepersToRemove;

    // Check for collisions between each spit and zookeeper
    for (const auto& spit : alpaca->GetSpits()) {
        for (const auto& zookeeper : zookeepers->GetBackgroundSprites()) {
            if (isColliding(spit.get(), zookeeper.get())) {
                spitsToRemove.push_back(spit.get());          // Mark spit for removal
                zookeepersToRemove.push_back(zookeeper.get()); // Mark zookeeper for removal
            }
        }
    }

    // Remove the collided spits from the alpaca's spit vector
    for (const auto& spitToRemove : spitsToRemove) {
        alpaca->RemoveSpit(spitToRemove);
    }

    // Remove the collided zookeepers from the zookeeper vector
    for (const auto& zookeeperToRemove : zookeepersToRemove) {
        zookeepers->GetBackgroundSprites().erase(
            std::remove_if(zookeepers->GetBackgroundSprites().begin(), zookeepers->GetBackgroundSprites().end(),
                [zookeeperToRemove](const std::unique_ptr<CSimpleSprite>& zookeeper) {
                    return zookeeper.get() == zookeeperToRemove;
                }), zookeepers->GetBackgroundSprites().end());
    }
}

//------------------------------------------------------------------------
// Handle collision between alpaca and zookeepers
// Detects collisions between the alpaca and zookeepers. If a collision
// occurs, zookeeper is "run over," and alpaca loses a heart and becomes
// temporarily invincible.
//------------------------------------------------------------------------
void HandleAlpacaZookeeperCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<Zookeepers>& zookeepers) {
    for (const auto& zookeeper : zookeepers->GetBackgroundSprites()) {
        if (isColliding(alpaca->GetSprite(), zookeeper.get())) {
            zookeeper->SetAngle(-PI / 2.2f);  // Rotate zookeeper to indicate it's "run over"

            // If alpaca is not invincible, it loses a heart and becomes invincible
            if (!alpaca->isInvincible) {
                App::PlaySound(".\\TestData\\Test.wav");  // Play sound on collision
                alpaca->hearts--;  // Reduce heart count
                alpaca->isInvincible = true;  // Make alpaca invincible
                alpaca->invisibleStartTime = std::chrono::steady_clock::now();  // Start invincibility timer
                alpaca->SetAngryAppearance();  // Change alpaca's appearance to show anger
            }
        }
    }
}

//------------------------------------------------------------------------
// Handle collision between alpaca and waterbottles
// Detects collisions between the alpaca and water bottles. Upon collision,
// the water bottle is removed, and the alpaca's spit count is restored.
//------------------------------------------------------------------------
void HandleAlpacaWaterBottleCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<WaterBottles>& waterbottles) {
    std::vector<CSimpleSprite*> waterBottlesToRemove;

    // Check for collisions between the alpaca and water bottles
    for (const auto& waterbottle : waterbottles->GetBackgroundSprites()) {
        if (isColliding(waterbottle.get(), alpaca->GetSprite())) {
            waterBottlesToRemove.push_back(waterbottle.get());  // Mark water bottle for removal

            // Restore alpaca's spit count if it's less than the maximum
            if (alpaca->spitsCount < 5) {
                alpaca->spitsCount++;
            }
        }
    }

    // Remove the collided water bottles from the water bottle vector
    for (const auto& waterBottleToRemove : waterBottlesToRemove) {
        waterbottles->GetBackgroundSprites().erase(
            std::remove_if(waterbottles->GetBackgroundSprites().begin(), waterbottles->GetBackgroundSprites().end(),
                [waterBottleToRemove](const std::unique_ptr<CSimpleSprite>& waterbottle) {
                    return waterbottle.get() == waterBottleToRemove;
                }), waterbottles->GetBackgroundSprites().end());
    }
}
