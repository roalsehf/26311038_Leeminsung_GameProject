#include "Player.h"

#include <glc2d.h>

void Player::SetTexture(int textureKey)
{
    textureKey_ = textureKey;
}

int Player::GetHp() const
{
    return hp_;
}

int Player::GetMaxHp() const
{
    return maxHp_;
}

int Player::GetEnergy() const
{
    return energy_;
}

void Player::Draw() const
{
    if (textureKey_ < 0)
    {
        return;
    }

    VEC2 position(650.0f, 225.0f);
    VEC2 scale(0.26f, 0.26f);

    g2_DrawAlphaOption(1);
    g2_Draw2D(textureKey_, nullptr, &position, &scale);
    g2_DrawAlphaOption(0);
}
