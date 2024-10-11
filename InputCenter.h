#pragma once

#include <Vector2.h>

class InputCenter
{
public:
    InputCenter(InputCenter&) = delete;
    InputCenter(InputCenter&&) = delete;
    InputCenter& operator=(const InputCenter&) = delete;
    InputCenter& operator=(const InputCenter&&) = delete;

    static InputCenter* GetInstance() { static InputCenter instance; return &instance; }

    void UpdateState();

    char* GetKeyPtr() { return keys_; }
    char* GetPreKeyPtr() { return preKeys_; }

    void WASDMove(Vector2& _vec, float _speed) const;

private:
    InputCenter() = default;
    ~InputCenter() = default;

    char keys_[256];
    char preKeys_[256];
};