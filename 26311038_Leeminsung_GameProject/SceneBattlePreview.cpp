#include "SceneBattlePreview.h"

#include "CApplication.h"

void SceneBattlePreview::Reset()
{
    battle_.Reset();
    previousTime_ = g2_TimeGetTime();
    selectedResult_ = 0;
    attackKeySeconds_ = 0.0;
    guardKeySeconds_ = 0.0;
}

void SceneBattlePreview::Update(CApplication& application, const KEYCODE* keys)
{
    const long long now = g2_TimeGetTime();
    double elapsed = static_cast<double>(now - previousTime_) / 1000.0;
    previousTime_ = now;
    // A focus change must not skip an entire counterattack or result animation.
    if (elapsed < 0.0) elapsed = 0.0;
    if (elapsed > 0.1) elapsed = 0.1;
    if (GetForegroundWindow() != g2_GetHwnd())
    {
        return;
    }

    const BattlePhase phase = battle_.GetPhase();
    if (phase == BattlePhase::Won || phase == BattlePhase::Lost)
    {
        UpdateResult(application, keys);
        return;
    }

    if (CApplication::IsKeyPressed(keys, VK_ESCAPE))
    {
        application.ChangeScene(SceneId::MainMenu);
        return;
    }

    if (attackKeySeconds_ > 0.0) attackKeySeconds_ -= elapsed;
    if (guardKeySeconds_ > 0.0) guardKeySeconds_ -= elapsed;
    battle_.Update(elapsed);

    // Do not carry an input from a previous phase into a freshly opened turn.
    if (battle_.GetPhase() != phase)
    {
        return;
    }
    const bool attack = CApplication::IsKeyPressed(keys, 'J');
    const bool guard = CApplication::IsKeyPressed(keys, 'K');
    // One action per frame. During a counterattack only K is accepted.
    if (phase == BattlePhase::EnemyWindup && guard)
    {
        if (battle_.Guard()) guardKeySeconds_ = 0.25;
    }
    else if (attack)
    {
        if (battle_.Attack()) attackKeySeconds_ = 0.25;
    }
    else if (guard)
    {
        if (battle_.Guard()) guardKeySeconds_ = 0.25;
    }
}

void SceneBattlePreview::UpdateResult(CApplication& application, const KEYCODE* keys)
{
    if (CApplication::IsKeyPressed(keys, VK_UP) ||
        CApplication::IsKeyPressed(keys, VK_DOWN) ||
        CApplication::IsKeyPressed(keys, 'W') ||
        CApplication::IsKeyPressed(keys, 'S'))
    {
        selectedResult_ = 1 - selectedResult_;
    }
    if (CApplication::IsKeyPressed(keys, VK_ESCAPE))
    {
        application.ChangeScene(SceneId::MainMenu);
    }
    else if (CApplication::IsKeyPressed(keys, VK_RETURN))
    {
        if (selectedResult_ == 0)
        {
            Reset();
        }
        else
        {
            application.ChangeScene(SceneId::MainMenu);
        }
    }
}

void SceneBattlePreview::Render(const CApplication& application) const
{
    const GameResources& resources = application.GetResources();
    application.DrawFullScreenTexture(resources.battleBackground);
    const BattlePhase phase = battle_.GetPhase();
    if (phase == BattlePhase::Won || phase == BattlePhase::Lost)
    {
        DrawResult(application);
        return;
    }

    const Player& player = battle_.GetPlayer();
    int playerTexture = resources.playerTexture;
    if (phase == BattlePhase::EnemyFeedback)
    {
        playerTexture = battle_.LastGuardSucceeded()
            ? resources.playerGuard : resources.playerHit;
    }
    else if (battle_.IsGuardQueued())
    {
        playerTexture = resources.playerGuard;
    }

    const int enemyTexture = battle_.IsEnemyHitVisible() || phase == BattlePhase::EnemyDefeated
        ? resources.goblinHit : resources.goblin;
    const DWORD enemyColor = phase == BattlePhase::EnemyDefeated ? 0xFF777777 : 0xFFFFFFFF;
    application.DrawTexture(enemyTexture, { 42, 146, 378, 482 }, nullptr, enemyColor);

    // Keep the portrait frame aligned when switching normal, hit and guard art.
    // The normal source has a different canvas size; fit its frame without cropping outside it.
    const RECT normalDestination = { 646, 154, 998, 474 };
    application.DrawTexture(playerTexture,
        playerTexture == resources.playerTexture ? normalDestination : RECT{ 646, 146, 982, 482 });

    g2_FontDrawText(resources.headingFont, { 362, 28, 850, 72 },
        0xFFFFD166, "GOBLIN %d / %d", battle_.GetEnemyNumber(), CombatState::TotalEnemies);
    g2_FontDrawText(resources.menuFont, { 92, 92, 395, 126 },
        0xFFFF9D91, "GOBLIN");
    g2_FontDrawText(resources.menuFont, { 742, 92, 988, 126 },
        0xFF8CD8EB, "PLAYER");
    g2_FontDrawText(resources.bodyFont, { 92, 129, 397, 161 },
        0xFFFFFFFF, "HP %d / %d", battle_.GetEnemyHp(), battle_.GetEnemyMaxHp());
    g2_FontDrawText(resources.bodyFont, { 742, 129, 999, 161 },
        player.GetHp() <= 14 ? 0xFFFF7B7B : 0xFFFFFFFF,
        "HP %d / %d", player.GetHp(), player.GetMaxHp());

    if (phase == BattlePhase::EnemyWindup)
    {
        g2_FontDrawText(resources.bodyFont, { 392, 242, 690, 276 },
            0xFFFFD166, "COUNTERATTACK");
        g2_FontDrawText(resources.bodyFont, { 420, 280, 674, 314 },
            0xFFFFFFFF, "IN %.1f s", battle_.GetPhaseSecondsRemaining());
        g2_FontDrawText(resources.bodyFont, { 392, 324, 690, 358 },
            battle_.IsGuardQueued() ? 0xFF99E6AE : 0xFFFFD166,
            "%s", battle_.IsGuardQueued() ? "GUARD READY" : "PRESS K: GUARD");
    }
    else
    {
        const char* status = "YOUR TURN";
        DWORD color = 0xFFFFD166;
        if (phase == BattlePhase::EnemyDefeated) status = "DEFEATED";
        else if (phase == BattlePhase::EnemyFeedback)
        {
            status = battle_.LastGuardSucceeded() ? "BLOCKED" : "HIT";
            color = battle_.LastGuardSucceeded() ? 0xFF99E6AE : 0xFFFF7B7B;
        }
        g2_FontDrawText(resources.menuFont, { 404, 272, 704, 310 }, color, "%s", status);
    }

    g2_FontDrawText(resources.bodyFont, { 100, 482, 992, 516 },
        0xFFFFE7BD, "%s", BattleMessage());
    DrawActionKeys(application);
    g2_FontDrawText(resources.bodyFont, { 335, 596, 976, 630 },
        0xFFCDD6DF, "Esc: Main Menu   Defeated: %d / %d",
        battle_.GetDefeatedCount(), CombatState::TotalEnemies);
}

void SceneBattlePreview::DrawActionKeys(const CApplication& application) const
{
    const GameResources& resources = application.GetResources();
    const BattlePhase phase = battle_.GetPhase();
    // Each supplied key image is a sheet of three 19 x 21 frames.
    const int attackFrame = attackKeySeconds_ > 0.0 ? 1 : 0;
    const int guardFrame = guardKeySeconds_ > 0.0 || battle_.IsGuardQueued() ? 1 : 0;
    const RECT attackSource = { attackFrame * 19, 0, attackFrame * 19 + 19, 21 };
    const RECT guardSource = { guardFrame * 19, 0, guardFrame * 19 + 19, 21 };
    const DWORD attackColor = phase == BattlePhase::AwaitAttack ? 0xFFFFFFFF : 0xFF999999;
    const bool canGuard = phase == BattlePhase::AwaitAttack || phase == BattlePhase::EnemyWindup;
    const DWORD guardColor = canGuard ? 0xFFFFFFFF : 0xFF999999;
    application.DrawTexture(resources.attackKey, { 126, 539, 164, 581 }, &attackSource, attackColor);
    application.DrawTexture(resources.guardKey, { 552, 539, 590, 581 }, &guardSource, guardColor);
    g2_FontDrawText(resources.bodyFont, { 181, 548, 526, 582 },
        attackColor, "ATTACK: %d damage", CombatState::AttackDamage);
    g2_FontDrawText(resources.bodyFont, { 607, 548, 1004, 582 },
        guardColor, "GUARD: 60%% no damage");
}

const char* SceneBattlePreview::BattleMessage() const
{
    switch (battle_.GetPhase())
    {
    case BattlePhase::EnemyWindup:
        return battle_.IsGuardQueued()
            ? "Guard selected. One 60% chance for this counterattack."
            : "The goblin will strike back. Press K before the attack lands.";
    case BattlePhase::EnemyFeedback:
        if (battle_.LastGuardSucceeded()) return "Guard succeeded. You took no damage.";
        return battle_.WasGuardAttempted()
            ? "Guard failed. You took 7 damage." : "You took 7 damage. Use K to guard the next counterattack.";
    case BattlePhase::EnemyDefeated:
        return "Goblin defeated.";
    default:
        return "Press J to attack. Press K during the counterattack to guard.";
    }
}

void SceneBattlePreview::DrawResult(const CApplication& application) const
{
    const GameResources& resources = application.GetResources();
    application.DrawTexture(resources.battleBackground, { 0, 0, 1024, 640 }, nullptr, 0xFF555555);
    const bool won = battle_.GetPhase() == BattlePhase::Won;
    g2_FontDrawText(resources.headingFont, { 386, 100, 820, 144 },
        won ? 0xFFFFD166 : 0xFFFF8888, "%s", won ? "ALL CLEAR" : "GAME OVER");
    g2_FontDrawText(resources.bodyFont, { 306, 181, 900, 220 },
        0xFFE6EDF3, "Goblins defeated: %d / %d", battle_.GetDefeatedCount(), CombatState::TotalEnemies);
    g2_FontDrawText(resources.bodyFont, { 306, 223, 900, 260 },
        0xFFE6EDF3, "Player HP: %d / %d", battle_.GetPlayer().GetHp(), battle_.GetPlayer().GetMaxHp());

    const char* choices[] = { "RETRY", "MAIN MENU" };
    for (int index = 0; index < 2; ++index)
    {
        const int top = 330 + index * 68;
        g2_FontDrawText(resources.menuFont, { 420, top, 812, top + 44 },
            selectedResult_ == index ? 0xFFFFD166 : 0xFFD4DFE8, "%s", choices[index]);
    }
    g2_FontDrawText(resources.bodyFont, { 252, 526, 960, 562 },
        0xFFCDD6DF, "W/S or Arrows: Move   Enter: Select");
    g2_FontDrawText(resources.bodyFont, { 252, 569, 960, 605 },
        0xFFCDD6DF, "Retry starts from Goblin 1 with full HP.");
}
