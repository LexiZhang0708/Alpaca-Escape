#pragma once
#include <memory>
#include "Alpaca.h"
#include "BackgroundObj.h"

// Function to handle spit and zookeeper collision
void HandleSpitZookeepersCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<Zookeepers>& zookeepers);

// Function to handle alpaca and zookeeper collision
void HandleAlpacaZookeeperCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<Zookeepers>& zookeepers);

// Function to handle alpaca and waterbottle collision
void HandleAlpacaWaterBottleCollision(std::unique_ptr<Alpaca>& alpaca, std::unique_ptr<WaterBottles>& waterbottles);
