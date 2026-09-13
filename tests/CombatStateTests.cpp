#include "CombatState.h"

#include <iostream>
#include <limits>
#include <stdexcept>

namespace
{
    void Check(bool condition, const char* message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }

    void TestPlayerDamage()
    {
        Player player;
        Check(player.GetHp() == 50 && player.GetMaxHp() == 50, "Initial player HP");
        player.TakeDamage(7);
        Check(player.GetHp() == 43, "Player damage");
        player.TakeDamage(-10);
        player.TakeDamage(0);
        Check(player.GetHp() == 43, "Negative damage must not heal");
        player.TakeDamage(std::numeric_limits<int>::max());
        Check(player.GetHp() == 0, "Damage must clamp at zero");
        player.Reset();
        Check(player.GetHp() == 50, "Player reset");
    }

    void TestAttackAndFeedback()
    {
        CombatState battle(1);
        Check(battle.Attack(), "First J must attack");
        Check(battle.GetEnemyHp() == 24, "J must deal eight damage");
        Check(battle.IsEnemyHitVisible(), "Attack must show goblin hit");
        Check(!battle.Attack(), "Repeated J during windup must be ignored");
        battle.Update(0.4);
        Check(!battle.IsEnemyHitVisible(), "Goblin hit must end before windup");
        battle.Update(0.71);
        Check(battle.GetPhase() == BattlePhase::EnemyFeedback, "Enemy feedback phase");
        Check(battle.GetPlayer().GetHp() == 43, "Unguarded attack must deal seven damage");
        Check(!battle.Attack() && !battle.Guard(), "Feedback must lock actions");
        battle.Update(1.0);
        Check(battle.GetPhase() == BattlePhase::AwaitAttack, "Return to player input");
        Check(battle.GetPlayer().GetHp() == 43, "Enemy damage must apply only once");
    }

    void TestGuardRolls()
    {
        CombatState normal(42);
        CombatState repeated(42);
        int successes = 0;
        constexpr int Trials = 20000;

        for (int trial = 0; trial < Trials; ++trial)
        {
            normal.Reset();
            repeated.Reset();
            Check(normal.Guard() && repeated.Guard(), "K must start a defensive turn");
            Check(normal.GetEnemyHp() == 32, "Guard must not attack");
            Check(!normal.IsEnemyHitVisible(), "Guard must not show goblin hit");
            Check(normal.IsGuardQueued(), "Guard must be queued");
            for (int extra = 0; extra < 10; ++extra)
            {
                Check(!repeated.Guard(), "Repeated K must not queue another roll");
            }
            normal.Update(1.11);
            repeated.Update(1.11);

            Check(normal.WasGuardAttempted(), "Attempt flag at enemy attack");
            Check(!normal.IsGuardQueued(), "Guard must be consumed");
            Check(normal.LastGuardSucceeded() == repeated.LastGuardSucceeded(),
                "Repeated K must not change RNG sequence");
            const bool guarded = normal.LastGuardSucceeded();
            Check(normal.GetPlayer().GetHp() == (guarded ? 50 : 43),
                "Guard success prevents all damage; failure takes full damage");
            successes += guarded ? 1 : 0;
        }

        const double rate = static_cast<double>(successes) / Trials;
        Check(rate > 0.58 && rate < 0.62, "Fixed-seed guard rate should be close to 60 percent");
        std::cout << "Guard successes: " << successes << '/' << Trials << '\n';
    }

    void TestEnemyDefeat()
    {
        CombatState battle(5);
        for (int hit = 0; hit < 3; ++hit)
        {
            Check(battle.Attack(), "Attack before killing blow");
            battle.Update(10.0);
        }
        const int previousHp = battle.GetPlayer().GetHp();
        battle.Attack();
        Check(battle.GetEnemyHp() == 0, "Killing blow must reach zero enemy HP");
        Check(battle.GetDefeatedCount() == 1, "Defeat count must increase once");
        Check(battle.GetPhase() == BattlePhase::EnemyDefeated, "Defeat feedback phase");
        Check(!battle.Guard() && !battle.Attack(), "Dead enemy must not accept actions");
        battle.Update(10.0);
        Check(battle.GetPlayer().GetHp() == previousHp, "Dead enemy must not retaliate");
        Check(battle.GetEnemyNumber() == 2 && battle.GetEnemyHp() == 32,
            "Next goblin starts with full HP");
        Check(battle.GetPhase() == BattlePhase::AwaitAttack, "Next goblin awaits input");
    }

    void TestWinLoseAndRestart()
    {
        CombatState battle(42);
        for (int turn = 0; turn < 12; ++turn)
        {
            Check(battle.Attack(), "Attack through three-goblin fight");
            if (battle.GetPhase() == BattlePhase::EnemyWindup)
            {
                Check(battle.Guard(), "K during windup must queue guard");
            }
            battle.Update(10.0);
        }
        Check(battle.GetPhase() == BattlePhase::Won, "All three goblins must lead to victory");
        Check(battle.GetDefeatedCount() == 3 && battle.GetEnemyNumber() == 3,
            "Victory enemy counters");
        const int winHp = battle.GetPlayer().GetHp();
        Check(!battle.Attack() && !battle.Guard(), "Victory must ignore battle input");
        battle.Update(1000.0);
        Check(battle.GetPhase() == BattlePhase::Won && battle.GetPlayer().GetHp() == winHp,
            "Victory must be terminal");

        battle.Reset();
        for (int turn = 0; turn < 12 && battle.GetPhase() != BattlePhase::Lost; ++turn)
        {
            Check(battle.Attack(), "Unguarded battle attack");
            battle.Update(10.0);
        }
        Check(battle.GetPhase() == BattlePhase::Lost && battle.GetPlayer().GetHp() == 0,
            "Zero HP must lead to defeat");
        const int lostEnemyHp = battle.GetEnemyHp();
        Check(!battle.Attack() && !battle.Guard(), "Defeat must ignore battle input");
        battle.Update(1000.0);
        Check(battle.GetPhase() == BattlePhase::Lost && battle.GetEnemyHp() == lostEnemyHp,
            "Defeat must be terminal");

        battle.Reset();
        Check(battle.GetPlayer().GetHp() == 50 && battle.GetEnemyHp() == 32,
            "Restart restores both HP values");
        Check(battle.GetEnemyNumber() == 1 && battle.GetDefeatedCount() == 0,
            "Restart resets enemy counters");
        Check(battle.GetPhase() == BattlePhase::AwaitAttack
            && battle.GetPhaseSecondsRemaining() == 0.0, "Restart resets phase");
        Check(!battle.IsGuardQueued() && !battle.WasGuardAttempted()
            && !battle.LastGuardSucceeded() && !battle.IsEnemyHitVisible(),
            "Restart clears visual feedback and guard state");

        battle.Attack();
        battle.Update(-1.0);
        battle.Update(std::numeric_limits<double>::quiet_NaN());
        Check(battle.GetPhase() == BattlePhase::EnemyWindup
            && battle.GetPlayer().GetHp() == 50, "Invalid elapsed times must be ignored");
        battle.Guard();
        battle.Reset();
        Check(!battle.IsGuardQueued() && !battle.IsEnemyHitVisible(),
            "Leaving mid-turn and restarting must clear pending actions");
    }
}

int main()
{
    try
    {
        TestPlayerDamage();
        TestAttackAndFeedback();
        TestGuardRolls();
        TestEnemyDefeat();
        TestWinLoseAndRestart();
        std::cout << "All combat tests passed.\n";
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
