#pragma once
#include "enemy.hpp"
#include "gameobject.hpp"
#include "context.hpp"
#include <SFML/Graphics.hpp>

struct EnemyBossData;

class EnemyBoss : public Enemy
{
public:
    EnemyBoss(const EnemyBossData &data, float x, float y, int difficulty);
    ~EnemyBoss() = default;
    void update(Context &context) override;

private:
    const sf::Texture &m_image;
    bool doing_attack;
    int attack_nr{};
    int counter{};
    int boss_attack_length{};
    float attack_time;

    void attack(Context &context);
    void attack0(Context &context);
    void attack1(Context &context);
    void attack2(Context &context);
    void spawn_powerups(Context &context);
};
