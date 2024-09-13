#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <string>
#include "app\app.h"

class GameObject {
protected:
    float posX, posY;
    const float speed;
    std::shared_ptr<CSimpleSprite> sprite;

public:
    GameObject(float speed, const std::string& spritePath);
    virtual ~GameObject() = default;

    virtual void UpdatePosition(float deltaTime) = 0;

    void SetPosition(float x, float y);
    void Draw();
    float GetPosX() const;
    float GetPosY() const;
    std::shared_ptr<CSimpleSprite> GetSprite() const;
};

#endif
