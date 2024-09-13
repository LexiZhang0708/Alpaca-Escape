#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include "app/app.h"

class Alpaca {
public:
    Alpaca();
    void AddSpit();
    void UpdateAlpacaPos(float deltaTime);
    void UpdateSpitPos(float deltaTime);
    void Render();
    void RemoveSpit(CSimpleSprite* spitToRemove);
    void ResetInvincibility();
    void SetAngryAppearance();
    void DrawSpitsCount();
    void DrawHeartCount();

    const std::vector<std::unique_ptr<CSimpleSprite>>& GetSpits() const;
    CSimpleSprite* GetSprite() const;

    const float ALPACA_SPEED = 0.5f;
    const float SPIT_SPEED = 0.7f;
    int hearts = 3;
    int spitsCount = 5;
    bool isInvincible = false;
    std::chrono::steady_clock::time_point invisibleStartTime;

private:
    std::unique_ptr<CSimpleSprite> alpacaSprite;
    std::vector<std::unique_ptr<CSimpleSprite>> spits;
};
