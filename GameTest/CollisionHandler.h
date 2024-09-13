#pragma once
#include <memory>
#include "Alpaca.h"
#include "BackgroundObj.h"

//------------------------------------------------------------------------
// CollisionHandler.h
// Functions to handle collisions between the Alpaca, Zookeepers, and 
// WaterBottles in the game.
//
// This header provides collision detection and response functions, ensuring
// proper interaction between the alpaca's spits, zookeepers, and water bottles.
//------------------------------------------------------------------------

// Handles collision between alpaca spits and zookeepers
void HandleSpitZookeepersCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<Zookeepers>& zookeepers);

// Handles collision between the alpaca and zookeepers
void HandleAlpacaZookeeperCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<Zookeepers>& zookeepers);

// Handles collision between the alpaca and water bottles
void HandleAlpacaWaterBottleCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<WaterBottles>& waterbottles);
