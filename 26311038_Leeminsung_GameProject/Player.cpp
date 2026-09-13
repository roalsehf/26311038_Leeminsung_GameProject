#include "Player.h"

#include <algorithm>

void Player::Reset()
{
    hp_ = MaxHp;
}

void Player::TakeDamage(int damage)
{
    if (damage > 0)
    {
        hp_ = std::max(0, hp_ - damage);
    }
}

int Player::GetHp() const
{
    return hp_;
}

int Player::GetMaxHp() const
{
    return MaxHp;
}
