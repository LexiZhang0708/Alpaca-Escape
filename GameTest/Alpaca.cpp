#include <stdafx.h>
#include "Alpaca.h"

Alpaca::Alpaca() {
    alpacaSprite = std::unique_ptr<CSimpleSprite>(App::CreateSprite(".\\TestData\\alpaca.png", 1, 1));
    alpacaSprite->SetPosition(100.0f, 400.0f);
}

void Alpaca::AddSpit() {
    auto spit = std::unique_ptr<CSimpleSprite>(App::CreateSprite(".\\TestData\\spit.png", 1, 1));
    float pos_x, pos_y;
    alpacaSprite->GetPosition(pos_x, pos_y);
    spit->SetPosition(pos_x + 35, pos_y + 25);
    spits.push_back(std::move(spit));
}

void Alpaca::UpdateAlpacaPos(float deltaTime) {
    float x, y;
    alpacaSprite->GetPosition(x, y);
    if (App::GetController().GetLeftThumbStickX() > 0.5f) {
        x += ALPACA_SPEED * deltaTime;
    }
    if (App::GetController().GetLeftThumbStickX() < -0.5f) {
        x -= ALPACA_SPEED * deltaTime;
    }
    if (App::GetController().GetLeftThumbStickY() > 0.5f) {
        y += ALPACA_SPEED * deltaTime;
    }
    if (App::GetController().GetLeftThumbStickY() < -0.5f) {
        y -= ALPACA_SPEED * deltaTime;
    }
    alpacaSprite->SetPosition(x, y);
}

void Alpaca::UpdateSpitPos(float deltaTime) {
    for (const auto& spit : spits) {
        float posX, posY;
        spit->GetPosition(posX, posY);
        posX += SPIT_SPEED * deltaTime;
        spit->SetPosition(posX, posY);
    }
}

void Alpaca::Render() {
    alpacaSprite->Draw();
    for (const auto& spit : spits) {
        spit->Draw();
    }
}

void Alpaca::RemoveSpit(CSimpleSprite* spitToRemove) {
    spits.erase(std::remove_if(spits.begin(), spits.end(),
        [spitToRemove](const std::unique_ptr<CSimpleSprite>& spit) {
            return spit.get() == spitToRemove;
        }), spits.end());
}

void Alpaca::ResetInvincibility() {
    isInvincible = false;
    alpacaSprite->SetColor(1.0f, 1.0f, 1.0f);
    alpacaSprite->SetAngle(0.0f);
    alpacaSprite->SetScale(1.0f);
}

void Alpaca::SetAngryAppearance() {
    alpacaSprite->SetColor(1.0f, 0.0f, 0.0f);
    alpacaSprite->SetAngle(-0.35f);
    alpacaSprite->SetScale(1.2f);
}

void Alpaca::DrawSpitsCount() {
    for (int i = 0; i < spitsCount; i++) {
        float posY = 700;
        float posX = 50 + 35 * i;
        auto spit = App::CreateSprite(".\\TestData\\spit.png", 1, 1);
        spit->SetScale(1.5f);
        spit->SetAngle(PI / 2.0f);
        spit->SetPosition(posX, posY);
        spit->Draw();
    }
}

void Alpaca::DrawHeartCount() {
    for (int i = 0; i < hearts; i++) {
        float posY = 630;
        float posX = 60 + 55 * i;
        auto heart = App::CreateSprite(".\\TestData\\heart.png", 1, 1);
        heart->SetScale(1.5f);
        heart->SetPosition(posX, posY);
        heart->Draw();
    }
}

const std::vector<std::unique_ptr<CSimpleSprite>>& Alpaca::GetSpits() const {
    return spits;
}

CSimpleSprite* Alpaca::GetSprite() const {
    return alpacaSprite.get();
}
