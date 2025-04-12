#pragma once
#include "enemy.hpp"
#include "gameobject.hpp"
#include "context.hpp"
#include <SFML/Graphics.hpp>

struct EnemyMinionData;

//Class definition of Enemytank that inherits from Enemy. This is a type of
//enemy whith more health.

class EnemyTank : public Enemy
{
public:
    EnemyTank(const EnemyMinionData &data, float x, float y, int difficulty);
    ~EnemyTank() = default;
    void update(Context &context) override;

private:
    const sf::Texture &m_image;
    void attack(Context &context) override;
};