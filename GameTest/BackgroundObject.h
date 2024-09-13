#ifndef BACKGROUNDOBJECT_H
#define BACKGROUNDOBJECT_H

#include "GameObject.h"
#include <vector>

class BackgroundObject : public GameObject {
protected:
    std::vector<std::shared_ptr<CSimpleSprite>> objects;

public:
    BackgroundObject(float speed, const std::string& spritePath);
    void AddObject(float posY);
    void UpdatePosition(float deltaTime) override;
    void Draw();
    const std::vector<std::shared_ptr<CSimpleSprite>>& GetObjects() const;
};

#endif
