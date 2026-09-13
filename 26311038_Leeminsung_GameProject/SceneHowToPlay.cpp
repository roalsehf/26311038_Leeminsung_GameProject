#include "SceneHowToPlay.h"

#include "CApplication.h"

void SceneHowToPlay::Update(CApplication& application, const KEYCODE* keys)
{
    if (CApplication::IsKeyPressed(keys, VK_RETURN) ||
        CApplication::IsKeyPressed(keys, VK_ESCAPE))
    {
        application.ChangeScene(SceneId::MainMenu);
    }
}

void SceneHowToPlay::Render(const CApplication& application) const
{
    const GameResources& resources = application.GetResources();
    application.DrawFullScreenTexture(resources.battleBackground);

    g2_FontDrawText(
        resources.headingFont,
        { 380, 38, 760, 88 },
        0xFFFFD166,
        "HOW TO PLAY");

    g2_FontDrawText(
        resources.bodyFont,
        { 115, 120, 920, 155 },
        0xFFE6EDF3,
        "Defeat three goblins in a row to clear the battle.");
    g2_FontDrawText(
        resources.bodyFont,
        { 115, 168, 920, 203 },
        0xFFE6EDF3,
        "J: Attack the goblin for 8 damage.");
    g2_FontDrawText(
        resources.bodyFont,
        { 115, 216, 920, 251 },
        0xFFE6EDF3,
        "K: Guard the counterattack. 60% chance to take no damage.");
    g2_FontDrawText(
        resources.bodyFont,
        { 115, 264, 920, 299 },
        0xFFE6EDF3,
        "Press K before the counterattack lands. One attempt per attack.");
    g2_FontDrawText(
        resources.bodyFont,
        { 115, 312, 920, 347 },
        0xFFE6EDF3,
        "A failed guard or an unguarded counterattack deals 7 damage.");
    g2_FontDrawText(
        resources.bodyFont,
        { 115, 400, 920, 435 },
        0xFF70E000,
        "Player HP: 50   Goblin HP: 32   Result: Retry or Main Menu");
    g2_FontDrawText(
        resources.bodyFont,
        { 310, 550, 820, 590 },
        0xFFB8C5D6,
        "Enter or Esc: Return to Main Menu");
}
