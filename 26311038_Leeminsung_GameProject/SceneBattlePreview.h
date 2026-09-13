#pragma once

#include "GameScene.h"
#include "CombatState.h"

class SceneBattlePreview final : public GameScene
{
public:
    void Reset();
    void Update(CApplication& application, const KEYCODE* keys) override;
    void Render(const CApplication& application) const override;

private:
    void UpdateResult(CApplication& application, const KEYCODE* keys);
    void DrawResult(const CApplication& application) const;
    void DrawActionKeys(const CApplication& application) const;
    const char* BattleMessage() const;

    CombatState battle_;
    long long previousTime_ = 0;
    int selectedResult_ = 0;
    double attackKeySeconds_ = 0.0;
    double guardKeySeconds_ = 0.0;
};
