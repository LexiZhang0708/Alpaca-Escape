#pragma once
#include "app/app.h"  

//------------------------------------------------------------------------
// Utils.h
// Utility functions used across the game.
//------------------------------------------------------------------------

// Generates a random float between 0.0f and 1.0f
float GenerateRandNum();

// Detects collision between two CSimpleSprite objects based on their positions and sizes
bool isColliding(CSimpleSprite* obj1, CSimpleSprite* obj2);
