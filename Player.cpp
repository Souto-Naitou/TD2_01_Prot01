#include "Player.h"

#include <Novice.h>

void Player::Initialize()
{
    startTime_ = std::chrono::system_clock::now();
    pEasingPlayerMove_ = std::make_unique<Easing>("PlayerMove", Easing::EaseType::EaseOutCubic, 0.5);
    pEasingPlayerMove_->Start();
}

void Player::Update()
{
    memcpy(preKeys, keys, 256);
    Novice::GetHitKeyStateAll(keys);

    if (index_ >= pCircle_->size()) index_ = 0;

    std::chrono::system_clock::time_point nowTime = std::chrono::system_clock::now();

    if (index_ == 0)
    {
        position_.Lerp((*pCircle_)[pCircle_->size() - 1], (*pCircle_)[index_], pEasingPlayerMove_->Update());
    }
    else
    {
        position_.Lerp((*pCircle_)[index_ - 1], (*pCircle_)[index_], pEasingPlayerMove_->Update());
    }
    
    if (std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - startTime_) > std::chrono::milliseconds(500))
    {
        index_++;
        startTime_ = std::chrono::system_clock::now();
        pEasingPlayerMove_->Reset();
        pEasingPlayerMove_->Start();
    }

}

void Player::Draw()
{
    Novice::DrawEllipse(
        static_cast<int>(position_.x),
        static_cast<int>(position_.y),
        20,
        20,
        0.0f,
        GREEN,
        kFillModeSolid
    );
}