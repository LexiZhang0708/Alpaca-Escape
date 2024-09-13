#include "Alpaca.h"
#include "app\app.h"

Alpaca::Alpaca() : GameObject(0.5f, ".\\TestData\\alpaca.png"), hearts(3), spitCount(5), isInvincible(false) {
    SetPosition(100.0f, 400.0f);
}

void Alpaca::UpdatePosition(float deltaTime) {
    // WASD movement logic with boundary checks
    if (App::GetController().GetLeftThumbStickX() > 0.5f && posX + sprite->GetWidth() / 2 < APP_VIRTUAL_WIDTH) {
        posX += speed * deltaTime;
    }
    if (App::GetController().GetLeftThumbStickX() < -0.5f && posX - sprite->GetWidth() / 2 > 0) {
        posX -= speed * deltaTime;
    }
    if (App::GetController().GetLeftThumbStickY() > 0.5f && posY + sprite->GetHeight() / 2 < APP_VIRTUAL_HEIGHT) {
        posY += speed * deltaTime;
    }
    if (App::GetController().GetLeftThumbStickY() < -0.5f && posY - sprite->GetHeight() / 2 > 0) {
        posY -= speed * deltaTime;
    }

    sprite->SetPosition(posX, posY);
}

