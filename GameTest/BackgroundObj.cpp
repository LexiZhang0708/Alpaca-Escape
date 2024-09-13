#include <stdafx.h>
#include "BackgroundObj.h"
#include "Utils.h"
#include <algorithm>

// Adds a new background object (e.g., zookeeper or water bottle) to the scene
void BackgroundObj::AddBackgroundObj() {
    // Create the background object sprite from the given image
    auto backgroundSprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite((".\\TestData\\" + imgName).c_str(), 1, 1));

    // Set the vertical position using clamped random positioning
    float pos_y = clamp(GenerateRandNum() * APP_VIRTUAL_HEIGHT, backgroundSprite->GetHeight() / 2.0f, APP_VIRTUAL_HEIGHT - backgroundSprite->GetHeight() / 2.0f);

    // Set the initial position of the object at the right edge of the screen
    backgroundSprite->SetPosition(APP_VIRTUAL_WIDTH, pos_y);

    // Add the new object to the vector of background sprites
    backgroundSprites.push_back(std::move(backgroundSprite));
}

// Removes background objects that have moved out of screen bounds
void BackgroundObj::RemoveOutOfBoundObjs() {
    // Remove objects that have completely moved off the left side of the screen
    backgroundSprites.erase(std::remove_if(backgroundSprites.begin(), backgroundSprites.end(),
        [](const std::unique_ptr<CSimpleSprite>& obj) {
            float posX, posY;
            obj->GetPosition(posX, posY);
            return posX + obj->GetWidth() / 2.0f < 0.0f;  // Check if the object is out of bounds
        }), backgroundSprites.end());
}

// Renders all background objects on the screen
void BackgroundObj::Render() {
    for (const auto& sprite : backgroundSprites) {
        sprite->Draw();  // Draw each background object
    }
}

// Updates the position of all background objects, moving them left at a given speed
void BackgroundObj::UpdateBackgroundObjPos(float deltaTime, float speed) {
    for (const auto& obj : backgroundSprites) {
        float posX, posY;
        obj->GetPosition(posX, posY);

        // Move the object left based on speed and deltaTime
        posX -= speed * deltaTime;

        // Set the new position for the object
        obj->SetPosition(posX, posY);
    }
}

// Returns a reference to the vector of background sprites
std::vector<std::unique_ptr<CSimpleSprite>>& BackgroundObj::GetBackgroundSprites() {
    return backgroundSprites;
}

// Clamps a value 'n' between 'min' and 'max'
float BackgroundObj::clamp(float n, float min, float max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

// Zookeepers constructor: Sets the image name for zookeepers
Zookeepers::Zookeepers(const std::string& imgName) {
    this->imgName = imgName;
}

// WaterBottles constructor: Sets the image name for water bottles
WaterBottles::WaterBottles(const std::string& imgName) {
    this->imgName = imgName;
}
