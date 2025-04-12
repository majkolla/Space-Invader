#pragma once
#include "enemy.hpp"
#include "gameobject.hpp"
#include "context.hpp"
#include <SFML/Graphics.hpp>

struct EnemyMinionData;

class EnemyMinion : public Enemy
{
public:
    EnemyMinion(const EnemyMinionData &data, float x, float y, int difficulty);
    ~EnemyMinion() = default;
    void update(Context &context) override;

private:
    const sf::Texture &m_image;

    void attack(Context &context) override;
};