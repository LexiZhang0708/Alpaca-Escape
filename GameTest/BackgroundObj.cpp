#include <stdafx.h>
#include "BackgroundObj.h"
#include "Utils.h"
#include <algorithm>  // for std::min and std::max

// AddBackgroundObj function
void BackgroundObj::AddBackgroundObj() {
    auto backgroundSprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite((".\\TestData\\" + imgName).c_str(), 1, 1));
    float pos_y = clamp(GenerateRandNum() * APP_VIRTUAL_HEIGHT, backgroundSprite->GetHeight() / 2.0f, APP_VIRTUAL_HEIGHT - backgroundSprite->GetHeight() / 2.0f);
    backgroundSprite->SetPosition(APP_VIRTUAL_WIDTH, pos_y);
    backgroundSprites.push_back(std::move(backgroundSprite));
}

// RemoveOutOfBoundObjs function
void BackgroundObj::RemoveOutOfBoundObjs() {
    backgroundSprites.erase(std::remove_if(backgroundSprites.begin(), backgroundSprites.end(),
        [](const std::unique_ptr<CSimpleSprite>& obj) {
            float posX, posY;
            obj->GetPosition(posX, posY);
            return posX + obj->GetWidth() / 2.0f < 0.0f;
        }), backgroundSprites.end());
}

// Render function
void BackgroundObj::Render() {
    for (const auto& sprite : backgroundSprites) {
        sprite->Draw();
    }
}

// UpdateBackgroundObjPos function
void BackgroundObj::UpdateBackgroundObjPos(float deltaTime, float speed) {
    for (const auto& obj : backgroundSprites) {
        float posX, posY;
        obj->GetPosition(posX, posY);
        posX -= speed * deltaTime;
        obj->SetPosition(posX, posY);
    }
}

// GetBackgroundSprites function
std::vector<std::unique_ptr<CSimpleSprite>>& BackgroundObj::GetBackgroundSprites() {
    return backgroundSprites;
}

// Fixed custom clamp function
float BackgroundObj::clamp(float n, float min, float max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

// Zookeepers constructor
Zookeepers::Zookeepers(const std::string& imgName) {
    this->imgName = imgName;
}

// WaterBottles constructor
WaterBottles::WaterBottles(const std::string& imgName) {
    this->imgName = imgName;
}
