#include "enemyminion.hpp"
#include "projectile.hpp"
#include "powerup.hpp"
#include "player.hpp"
#include "resourcemanager.hpp"
#include "gameconfiguration.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

EnemyMinion::EnemyMinion(const EnemyMinionData &data, float x, float y, int difficulty)
    : Enemy{data.base_health,
            data.base_speed,
            data.base_projectile_speed,
            data.base_attack_cooldown,
            data.base_projectile_prob,
            data.base_powerup_prob,
            data.base_speed * 0.5f},
      m_image{ResourceManager::load_texture("assets/images/enemy/enemy.png")}
{
    clock.restart();
    s_sprite.setTexture(m_image);
    sf::Vector2u texture_size{m_image.getSize()};
    s_sprite.setOrigin(texture_size.x / 2, texture_size.y / 2);
    s_sprite.setPosition(x, y);

    if (difficulty == 2)
    {
        s_health *= 2;
    }
    else if (difficulty == 3)
    {
        s_health *= 3;
        s_speed *= 1.5;
    }
    s_health_bar_offset = sf::Vector2f{0, (texture_size.y / 2.f + 10.f)};
    Ship::toggle_health_bar();
    s_max_health = s_health;
}

void EnemyMinion::update(Context &context)
{
    Ship::update_health_bar();
    Enemy::update(context);
}

void EnemyMinion::attack(Context &context)
{
    sf::Vector2f cur_pos = s_sprite.getPosition();
    context.spawn_object(new BasicProjectile{cur_pos.x, cur_pos.y, M_PI_2, s_projectile_speed, false});
}