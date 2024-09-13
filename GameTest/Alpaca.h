#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include "app/app.h"

//------------------------------------------------------------------------
// Alpaca Class
// This class represents the Alpaca object in the game. It handles the
// alpaca's movement, spitting functionality, invincibility, and rendering.
//------------------------------------------------------------------------
class Alpaca {
public:
    Alpaca();  // Initializes the alpaca object

    void AddSpit();  // Adds a spit to the alpaca's spit vector
    void UpdateAlpacaPos(float deltaTime);  // Updates alpaca's position based on input
    void UpdateSpitPos(float deltaTime);  // Updates positions of all spits
    void Render();  // Renders the alpaca and spits
    void RemoveSpit(CSimpleSprite* spitToRemove);  // Removes a specific spit
    void ResetInvincibility();  // Resets alpaca's invincibility state
    void SetAngryAppearance();  // Changes appearance to indicate invincibility
    void DrawSpitsCount();  // Displays current spit count on screen
    void DrawHeartCount();  // Displays current heart count on screen

    const std::vector<std::unique_ptr<CSimpleSprite>>& GetSpits() const;  // Returns list of active spits
    CSimpleSprite* GetSprite() const;  // Returns the alpaca sprite

    const float ALPACA_SPEED = 0.5f;  // Movement speed
    const float SPIT_SPEED = 0.7f;  // Spit speed
    int hearts = 3;  // Number of hearts (lives)
    int spitsCount = 5;  // Number of available spits
    bool isInvincible = false;  // Whether alpaca is invincible
    std::chrono::steady_clock::time_point invisibleStartTime;  // Time when invincibility started

private:
    std::unique_ptr<CSimpleSprite> alpacaSprite;  // The sprite representing the alpaca
    std::vector<std::unique_ptr<CSimpleSprite>> spits;  // Collection of active spits
};
