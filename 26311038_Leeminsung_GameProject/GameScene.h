#pragma once

#include <glc2d.h>

class GameApplication;

class GameScene
{
public:
    virtual ~GameScene() = default;

    virtual void Update(GameApplication& application, const KEYCODE* keys) = 0;
    virtual void Render(const GameApplication& application) const = 0;
};
