#include "BackgroundObject.h"

BackgroundObject::BackgroundObject(float speed, const std::string& spritePath)
    : GameObject(speed, spritePath) {}

void BackgroundObject::AddObject(float posY) {
    std::shared_ptr<CSimpleSprite> obj(App::CreateSprite(sprite->GetImageName(), 1, 1));
    obj->SetPosition(APP_VIRTUAL_WIDTH, posY);
    objects.push_back(obj);
}

void BackgroundObject::UpdatePosition(float deltaTime) {
    for (auto& obj : objects) {
        float objPosX, objPosY;
        obj->GetPosition(objPosX, objPosY);
        objPosX -= speed * deltaTime;
        obj->SetPosition(objPosX, objPosY);
    }

    // Remove objects that are out of bounds
    objects.erase(std::remove_if(objects.begin(), objects.end(),
        [](const std::shared_ptr<CSimpleSprite>& obj) {
            float posX, posY;
            obj->GetPosition(posX, posY);
            return posX < -obj->GetWidth() / 2.0f;
        }), objects.end());
}

void BackgroundObject::Draw() {
    for (const auto& obj : objects) {
        obj->Draw();
    }
}
