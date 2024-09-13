#ifndef ALPACA_H
#define ALPACA_H

#include "GameObject.h"
#include <vector>
#include <chrono>

class Alpaca : public GameObject {
private:
    int hearts;
    int spitCount;
    bool isInvincible;
    std::chrono::steady_clock::time_point invincibleStartTime;
    std::vector<std::shared_ptr<CSimpleSprite>> spits;

public:
    Alpaca();
    void UpdatePosition(float deltaTime) override;
    void AddSpit();
    void UpdateSpitPositions(float deltaTime);
    int GetHearts() const;
    void DecreaseHeart();
    bool IsInvincible() const;
    void SetInvincible(bool invincible);
    void UpdateInvincibility();
    void DrawStats();
};

#endif
