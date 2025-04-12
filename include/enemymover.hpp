#pragma once
#include "enemy.hpp"
#include "gameobject.hpp"
#include "context.hpp"
#include <SFML/Graphics.hpp>

struct EnemyMinionData;

class EnemyMover : public Enemy
{
public:
    EnemyMover(const EnemyMinionData &data, float x, float y, int difficulty);
    ~EnemyMover() = default;
    void update(Context &context) override;

private:
    const sf::Texture &m_image;
    sf::Time m_direction_change_time;

    void attack(Context &context) override;
};