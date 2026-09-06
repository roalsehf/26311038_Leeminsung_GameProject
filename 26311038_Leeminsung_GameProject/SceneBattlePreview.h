#pragma once

#include "GameScene.h"

class SceneBattlePreview final : public GameScene
{
public:
    void Update(CApplication& application, const KEYCODE* keys) override;
    void Render(const CApplication& application) const override;
};
