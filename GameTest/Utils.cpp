#include "stdafx.h"
#include "Utils.h"
#include <random>
#include <cmath>

// Generates a random float between 0.0 and 1.0 using a uniform distribution
float GenerateRandNum()
{
    std::random_device rd;  // Seed the random number generator
    std::mt19937 gen(rd());  // Mersenne Twister generator
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);  // Uniform distribution between 0 and 1
    return distribution(gen);  // Generate and return a random number
}

// Checks if two sprites are colliding based on their positions and dimensions
bool isColliding(CSimpleSprite* obj1, CSimpleSprite* obj2)
{
    // Get half-width and half-height for the first object
    float obj1Width = obj1->GetWidth() / 2.0f;
    float obj1Height = obj1->GetHeight() / 2.0f;
    float obj1X, obj1Y;
    obj1->GetPosition(obj1X, obj1Y);  // Get the position of the first object

    // Get half-width and half-height for the second object
    float obj2Width = obj2->GetWidth() / 2.0f;
    float obj2Height = obj2->GetHeight() / 2.0f;
    float obj2X, obj2Y;
    obj2->GetPosition(obj2X, obj2Y);  // Get the position of the second object

    // Check for overlap on both the x and y axes
    bool xCollision = std::abs(obj1X - obj2X) <= (obj1Width + obj2Width);  // X-axis overlap
    bool yCollision = std::abs(obj1Y - obj2Y) <= (obj1Height + obj2Height);  // Y-axis overlap

    return (xCollision && yCollision);  // True if there is a collision on both axes
}
