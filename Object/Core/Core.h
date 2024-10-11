#pragma once

#include <BaseObject.h>
#include <Rect2.h>

class Core : public BaseObject
{
public:

    Core() = default;
    ~Core() = default;

    void Initialize();
    void Update();
    void Draw();

private:
    Rect2 boxCore_;

};