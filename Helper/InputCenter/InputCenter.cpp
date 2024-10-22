#include "InputCenter.h"
#include "Novice.h"

void InputCenter::UpdateState()
{
    memcpy(preKeys_, keys_, 256);
    Novice::GetHitKeyStateAll(keys_);
}

void InputCenter::WASDMove(Vector2& _vec, float _speed) const
{
    if (keys_[DIK_W]) _vec.y += -_speed;
    if (keys_[DIK_S]) _vec.y += _speed;
    if (keys_[DIK_A]) _vec.x += -_speed;
    if (keys_[DIK_D]) _vec.x += _speed;
}
