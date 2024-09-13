#include <stdafx.h>
#include "Alpaca.h"

// Constructor: Creates the alpaca sprite and sets its initial position on the screen
Alpaca::Alpaca() {
    // Create the alpaca sprite from the specified image file
    alpacaSprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite(".\\TestData\\alpaca.png", 1, 1));

    // Set the initial position of the alpaca sprite
    alpacaSprite->SetPosition(100.0f, 400.0f);
}

// Adds a new spit to the alpaca's spit vector
void Alpaca::AddSpit() {
    // Create a new spit sprite
    auto spit = std::unique_ptr<CSimpleSprite>(App::CreateSprite(".\\TestData\\spit.png", 1, 1));

    // Get the current position of the alpaca so the spit can be positioned near its mouth
    float pos_x, pos_y;
    alpacaSprite->GetPosition(pos_x, pos_y);

    // Set the position of the spit relative to the alpaca's mouth
    spit->SetPosition(pos_x + 35, pos_y + 25);

    // Add the new spit to the vector of active spits
    spits.push_back(std::move(spit));
}

// Updates the alpaca's position based on controller input and deltaTime
void Alpaca::UpdateAlpacaPos(float deltaTime) {
    // Get the current position of the alpaca
    float x, y;
    alpacaSprite->GetPosition(x, y);

    // Check input from the left thumbstick and adjust alpaca's position accordingly
    if (App::GetController().GetLeftThumbStickX() > 0.5f) {
        x += ALPACA_SPEED * deltaTime;  // Move right
    }
    if (App::GetController().GetLeftThumbStickX() < -0.5f) {
        x -= ALPACA_SPEED * deltaTime;  // Move left
    }
    if (App::GetController().GetLeftThumbStickY() > 0.5f) {
        y += ALPACA_SPEED * deltaTime;  // Move up
    }
    if (App::GetController().GetLeftThumbStickY() < -0.5f) {
        y -= ALPACA_SPEED * deltaTime;  // Move down
    }

    // Set the new position for the alpaca sprite
    alpacaSprite->SetPosition(x, y);
}

// Updates the position of all active spits
void Alpaca::UpdateSpitPos(float deltaTime) {
    for (const auto& spit : spits) {
        float posX, posY;

        // Get the current position of the spit
        spit->GetPosition(posX, posY);

        // Move the spit to the right based on its speed and deltaTime
        posX += SPIT_SPEED * deltaTime;

        // Update the position of the spit
        spit->SetPosition(posX, posY);
    }
}

// Renders the alpaca and all active spits on the screen
void Alpaca::Render() {
    // Draw the alpaca sprite
    alpacaSprite->Draw();

    // Draw each spit sprite in the vector
    for (const auto& spit : spits) {
        spit->Draw();
    }
}

// Removes a specific spit from the vector of active spits
void Alpaca::RemoveSpit(CSimpleSprite* spitToRemove) {
    // Erase the spit from the vector by finding it and removing it
    spits.erase(std::remove_if(spits.begin(), spits.end(),
        [spitToRemove](const std::unique_ptr<CSimpleSprite>& spit) {
            return spit.get() == spitToRemove;  // Check if it's the spit to remove
        }), spits.end());
}

// Resets the alpaca's appearance after invincibility ends
void Alpaca::ResetInvincibility() {
    // Reset invincibility state
    isInvincible = false;

    // Restore alpaca's default appearance (color, angle, and scale)
    alpacaSprite->SetColor(1.0f, 1.0f, 1.0f);  // Restore color to white
    alpacaSprite->SetAngle(0.0f);  // Reset angle to neutral
    alpacaSprite->SetScale(1.0f);  // Reset size to normal
}

// Changes the alpaca's appearance to an "angry" state, indicating invincibility
void Alpaca::SetAngryAppearance() {
    // Set the alpaca's color to red, angle to tilted, and increase size
    alpacaSprite->SetColor(1.0f, 0.0f, 0.0f);  // Set color to red
    alpacaSprite->SetAngle(-0.35f);  // Tilt the alpaca
    alpacaSprite->SetScale(1.2f);  // Enlarge the alpaca
}

// Draws the number of spits available on the screen
void Alpaca::DrawSpitsCount() {
    // Loop through the number of available spits and draw each icon on the screen
    for (int i = 0; i < spitsCount; i++) {
        float posY = 700;
        float posX = 50 + 35 * i;

        // Create a new sprite to represent the spit count
        auto spit = App::CreateSprite(".\\TestData\\spit.png", 1, 1);
        spit->SetScale(1.5f);
        spit->SetAngle(PI / 2.0f);  // Rotate the sprite to face vertically
        spit->SetPosition(posX, posY);  // Position it on the screen
        spit->Draw();  // Draw the sprite
    }
}

// Draws the current heart (lives) count on the screen
void Alpaca::DrawHeartCount() {
    // Loop through the number of hearts and draw each heart on the screen
    for (int i = 0; i < hearts; i++) {
        float posY = 630;
        float posX = 60 + 55 * i;

        // Create a new sprite to represent the heart count
        auto heart = App::CreateSprite(".\\TestData\\heart.png", 1, 1);
        heart->SetScale(1.5f);  // Scale the heart icon
        heart->SetPosition(posX, posY);  // Position it on the screen
        heart->Draw();  // Draw the heart sprite
    }
}

// Returns a constant reference to the vector of active spits
const std::vector<std::unique_ptr<CSimpleSprite>>& Alpaca::GetSpits() const {
    return spits;
}

// Returns the alpaca's sprite pointer
CSimpleSprite* Alpaca::GetSprite() const {
    return alpacaSprite.get();
}
