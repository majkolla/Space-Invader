#pragma once
#include "enemy.hpp"
#include "gameobject.hpp"
#include "context.hpp"
#include <SFML/Graphics.hpp>

struct EnemyBossData;

class EnemyBoss2 : public Enemy
{
public:
    EnemyBoss2(const EnemyBossData &data, float x, float y, int diffuculty);
    ~EnemyBoss2() = default;
    void update(Context &context) override;

private:
    const sf::Texture &m_image;
    bool m_attack;
    float m_attack_time;
    int m_attack_number;
    int m_number_of_items_spawned;

    void check_health(Context &context);
    void attack(Context &context, int attack_number);
    void cooldown();
    void constant_attack(Context &context);

    sf::Clock const_attack_clock;
    sf::Clock m_clock;
};
