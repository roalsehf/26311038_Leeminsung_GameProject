#pragma once

#include "GameScene.h"

class SceneBattlePreview final : public GameScene
{
public:
    void Update(GameApplication& application, const KEYCODE* keys) override;
    void Render(const GameApplication& application) const override;
};
