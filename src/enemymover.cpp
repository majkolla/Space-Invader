#include "enemymover.hpp"
#include "resourcemanager.hpp"
#include "gameconfiguration.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

EnemyMover::EnemyMover(const EnemyMinionData &data, float x, float y, int difficulty)
    : Enemy{data.base_health,
            data.base_speed,
            data.base_projectile_speed,
            data.base_attack_cooldown * 0.5f,
            data.base_projectile_prob,
            data.base_powerup_prob * 2.0f,
            data.base_speed * 0.5f,
            {1 / M_SQRT1_2, 1 / M_SQRT2}},
      m_image{ResourceManager::load_texture("assets/images/enemy/enemy_mover.png")},
      m_direction_change_time{sf::seconds(0.5f)}
{
    s_speed *= difficulty;
    clock.restart();
    s_sprite.setTexture(m_image);
    sf::Vector2u texture_size{m_image.getSize()};
    s_sprite.setOrigin(texture_size.x / 2, texture_size.y / 2);
    s_sprite.setPosition(x, y);
    if (difficulty >= 3)
    {
        s_health *= 2;
    }
    s_health_bar_offset = sf::Vector2f{0, (texture_size.y / 2.f + 10.f)};
    Ship::toggle_health_bar();
    s_max_health = s_health;
}

void EnemyMover::update(Context &context)
{
    Ship::update_health_bar();
    sf::Vector2f old_pos = s_sprite.getPosition();
    Enemy::update(context);

    sf::Vector2f cur_pos = s_sprite.getPosition();
    // If the minion is outside the left or right of screen, it will change x direction.
    if (cur_pos.x < 0 || cur_pos.x > context.get_window_size().x)
    {
        e_move_direction.x *= -1;
        s_sprite.setPosition(old_pos);
    }
}

void EnemyMover::attack(Context &)
{
    //Mover attack is changing its moving direction
    e_move_direction.x *= -1;
}