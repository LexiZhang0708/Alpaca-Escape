#pragma once
#include "app/app.h"  

// Function to generate a random float between 0.0f and 1.0f
float GenerateRandNum();

// Function to detect collision between two CSimpleSprite objects
bool isColliding(CSimpleSprite* obj1, CSimpleSprite* obj2);
