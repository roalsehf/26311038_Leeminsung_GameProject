#include "SceneBattlePreview.h"

#include "CApplication.h"

void SceneBattlePreview::Update(CApplication& application, const KEYCODE* keys)
{
    if (CApplication::IsKeyPressed(keys, VK_RETURN) ||
        CApplication::IsKeyPressed(keys, VK_ESCAPE))
    {
        application.ChangeScene(SceneId::MainMenu);
    }
}

void SceneBattlePreview::Render(const CApplication& application) const
{
    const GameResources& resources = application.GetResources();
    const Player& player = application.GetPlayer();

    application.DrawFullScreenTexture(resources.battleBackground);
    player.Draw();

    g2_FontDrawText(
        resources.headingFont,
        { 405, 28, 760, 78 },
        0xFFFFD166,
        "STAGE 1");
    g2_FontDrawText(
        resources.headingFont,
        { 75, 110, 450, 155 },
        0xFFFF7B7B,
        "TRAINING GOBLIN");
    g2_FontDrawText(
        resources.bodyFont,
        { 80, 165, 430, 200 },
        0xFFE6EDF3,
        "HP: 32 / 32");
    g2_FontDrawText(
        resources.bodyFont,
        { 80, 205, 430, 240 },
        0xFFE6EDF3,
        "Next Action: Attack 7");

    g2_FontDrawText(
        resources.headingFont,
        { 720, 110, 960, 155 },
        0xFF48CAE4,
        "PLAYER");
    g2_FontDrawText(
        resources.bodyFont,
        { 735, 165, 990, 200 },
        0xFFE6EDF3,
        "HP: %d / %d",
        player.GetHp(),
        player.GetMaxHp());
    g2_FontDrawText(
        resources.bodyFont,
        { 735, 205, 990, 240 },
        0xFFE6EDF3,
        "Energy: %d / 3",
        player.GetEnergy());

    g2_FontDrawText(
        resources.bodyFont,
        { 70, 455, 590, 490 },
        0xFFFFD166,
        "Week 2: Texture Load and Draw");
    g2_FontDrawText(
        resources.bodyFont,
        { 70, 500, 600, 535 },
        0xFFB8C5D6,
        "Card battle rules will be connected next week.");
    g2_FontDrawText(
        resources.bodyFont,
        { 310, 570, 820, 605 },
        0xFFB8C5D6,
        "Enter or Esc: Return to Main Menu");
}
