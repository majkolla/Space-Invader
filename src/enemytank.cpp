#include "enemytank.hpp"
#include "projectile.hpp"
#include "resourcemanager.hpp"
#include "gameconfiguration.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

//EnemyTank inherits from enemy whith, speed, health and so on loaded from file.

EnemyTank::EnemyTank(const EnemyMinionData &data,
                    float x, float y,
                    int difficulty)
    : Enemy{data.base_health * 5,
            data.base_speed * 0.40f,
            data.base_projectile_speed,
            data.base_attack_cooldown * 2.0f,
            data.base_projectile_prob / 2.0f,
            data.base_powerup_prob * 3.0f,
            data.base_speed * 0.5f},
      m_image{ResourceManager::load_texture("assets/images/enemy/Tank.png")}
{
    //Based on difficulty the EnemyTank has a different amount of health.
    clock.restart();
    s_sprite.setTexture(m_image);
    sf::Vector2u texture_size{m_image.getSize()};
    s_sprite.setOrigin(texture_size.x / 2, texture_size.y / 2);
    s_sprite.setPosition(x, y);
    if (difficulty == 1)
    {
        s_health *= 1.5f;
    }
    else if (difficulty == 2)
    {
        s_health *= 2.0f;
    }
    else
    {
        s_speed *= 1.5f;
    }
    s_health_bar_offset = sf::Vector2f{0, (texture_size.y / 2.f + 10.f)};
    Ship::toggle_health_bar();
    s_max_health = s_health;
}

void EnemyTank::update(Context &context)
{
    Ship::update_health_bar();
    Enemy::update(context);
}

void EnemyTank::attack(Context &context)
{
    sf::Vector2f cur_pos = s_sprite.getPosition();
    context.spawn_object(new BasicProjectile{cur_pos.x,
                                            cur_pos.y,
                                            M_PI_2,
                                            s_projectile_speed,
                                            false});
}