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
        "Draw five cards at the beginning of each player turn.");
    g2_FontDrawText(
        resources.bodyFont,
        { 115, 168, 920, 203 },
        0xFFE6EDF3,
        "Use Energy to play Attack and Skill cards.");
    g2_FontDrawText(
        resources.bodyFont,
        { 115, 216, 920, 251 },
        0xFFE6EDF3,
        "Strike deals damage. Guard adds Block. Focus draws a card.");
    g2_FontDrawText(
        resources.bodyFont,
        { 115, 264, 920, 299 },
        0xFFE6EDF3,
        "End the turn to let the enemy perform its displayed action.");
    g2_FontDrawText(
        resources.bodyFont,
        { 115, 312, 920, 347 },
        0xFFE6EDF3,
        "Defeat the Dungeon Warden in Stage 3 to clear the game.");
    g2_FontDrawText(
        resources.bodyFont,
        { 115, 400, 920, 435 },
        0xFF70E000,
        "Starting HP: 50   Energy: 3   Hand: 5 cards");
    g2_FontDrawText(
        resources.bodyFont,
        { 310, 550, 820, 590 },
        0xFFB8C5D6,
        "Enter or Esc: Return to Main Menu");
}
