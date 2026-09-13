#include "CombatState.h"

#include <algorithm>
#include <cmath>

CombatState::CombatState()
    : CombatState(std::random_device{}())
{
}

CombatState::CombatState(unsigned int seed)
    : random_(seed)
{
    Reset();
}

void CombatState::Reset()
{
    player_.Reset();
    phase_ = BattlePhase::AwaitAttack;
    enemyHp_ = EnemyMaxHp;
    enemyNumber_ = 1;
    defeatedCount_ = 0;
    phaseSeconds_ = 0.0;
    guardQueued_ = false;
    guardAttempted_ = false;
    guardSucceeded_ = false;
    enemyWasHit_ = false;
}

bool CombatState::Attack()
{
    if (phase_ != BattlePhase::AwaitAttack)
    {
        return false;
    }

    enemyHp_ = std::max(0, enemyHp_ - AttackDamage);
    BeginEnemyTurn(true);

    if (enemyHp_ == 0)
    {
        ++defeatedCount_;
        phase_ = BattlePhase::EnemyDefeated;
        phaseSeconds_ = DefeatedSeconds;
    }

    return true;
}

bool CombatState::Guard()
{
    if (phase_ == BattlePhase::AwaitAttack)
    {
        BeginEnemyTurn(false);
    }

    if (phase_ != BattlePhase::EnemyWindup || guardQueued_)
    {
        return false;
    }

    // One guard attempt belongs to one enemy attack, even if K is pressed again.
    guardQueued_ = true;
    return true;
}

void CombatState::BeginEnemyTurn(bool enemyWasHit)
{
    phase_ = BattlePhase::EnemyWindup;
    phaseSeconds_ = WindupSeconds;
    guardQueued_ = false;
    guardAttempted_ = false;
    guardSucceeded_ = false;
    enemyWasHit_ = enemyWasHit;
}

void CombatState::Update(double seconds)
{
    if (seconds <= 0.0 || !std::isfinite(seconds))
    {
        return;
    }

    // Carry a slow frame through timed feedback, but stop when input is needed.
    while (seconds > 0.0 && phaseSeconds_ > 0.0)
    {
        if (seconds < phaseSeconds_)
        {
            phaseSeconds_ -= seconds;
            return;
        }

        seconds -= phaseSeconds_;
        phaseSeconds_ = 0.0;
        FinishPhase();
    }
}

void CombatState::FinishPhase()
{
    switch (phase_)
    {
    case BattlePhase::EnemyWindup:
        guardAttempted_ = guardQueued_;
        guardSucceeded_ = guardAttempted_
            && std::bernoulli_distribution(GuardChance)(random_);
        guardQueued_ = false;
        enemyWasHit_ = false;

        if (!guardSucceeded_)
        {
            player_.TakeDamage(EnemyDamage);
        }

        // Show the hit/guard image before entering the next turn or defeat screen.
        phase_ = BattlePhase::EnemyFeedback;
        phaseSeconds_ = FeedbackSeconds;
        break;

    case BattlePhase::EnemyFeedback:
        phase_ = player_.GetHp() == 0 ? BattlePhase::Lost : BattlePhase::AwaitAttack;
        break;

    case BattlePhase::EnemyDefeated:
        enemyWasHit_ = false;
        if (defeatedCount_ == TotalEnemies)
        {
            phase_ = BattlePhase::Won;
        }
        else
        {
            ++enemyNumber_;
            enemyHp_ = EnemyMaxHp;
            phase_ = BattlePhase::AwaitAttack;
        }
        break;

    default:
        break;
    }
}

BattlePhase CombatState::GetPhase() const
{
    return phase_;
}

const Player& CombatState::GetPlayer() const
{
    return player_;
}

int CombatState::GetEnemyHp() const
{
    return enemyHp_;
}

int CombatState::GetEnemyMaxHp() const
{
    return EnemyMaxHp;
}

int CombatState::GetEnemyNumber() const
{
    return enemyNumber_;
}

int CombatState::GetDefeatedCount() const
{
    return defeatedCount_;
}

bool CombatState::IsGuardQueued() const
{
    return guardQueued_;
}

bool CombatState::WasGuardAttempted() const
{
    return guardAttempted_;
}

bool CombatState::LastGuardSucceeded() const
{
    return guardSucceeded_;
}

double CombatState::GetPhaseSecondsRemaining() const
{
    return phaseSeconds_;
}

bool CombatState::IsEnemyHitVisible() const
{
    if (!enemyWasHit_)
    {
        return false;
    }

    const double phaseDuration = phase_ == BattlePhase::EnemyDefeated
        ? DefeatedSeconds : WindupSeconds;
    return phaseSeconds_ > phaseDuration - HitSeconds;
}
