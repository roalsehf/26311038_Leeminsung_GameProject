#pragma once

class Player
{
public:
    void SetTexture(int textureKey);

    int GetHp() const;
    int GetMaxHp() const;
    int GetEnergy() const;

    void Draw() const;

private:
    int textureKey_ = -1;
    int hp_ = 50;
    int maxHp_ = 50;
    int energy_ = 3;
};
