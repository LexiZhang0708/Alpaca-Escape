#include "GameObject.h"

GameObject::GameObject(float speed, const std::string& spritePath)
    : speed(speed) {
    sprite = std::shared_ptr<CSimpleSprite>(App::CreateSprite(spritePath.c_str(), 1, 1));
}

void GameObject::SetPosition(float x, float y) {
    posX = x;
    posY = y;
    sprite->SetPosition(posX, posY);
}

void GameObject::Draw() {
    sprite->Draw();
}

float GameObject::GetPosX() const {
    return posX;
}

float GameObject::GetPosY() const {
    return posY;
}

std::shared_ptr<CSimpleSprite> GameObject::GetSprite() const {
    return sprite;
}
