#include "enemyboss2.hpp"
#include "projectile.hpp"
#include "player.hpp"
#include "powerup.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include "resourcemanager.hpp"
#include "gameconfiguration.hpp"

EnemyBoss2::EnemyBoss2(const EnemyBossData &data, float x, float y, int)
    : Enemy{data.base_health,
            0.f,
            data.base_projectile_speed * 0.4f,
            data.base_attack_cooldown * 0.35f,
            0.f,
            0.f,
            0.f},
      m_image{ResourceManager::load_texture("assets/images/enemy/shrek1.png")},
      m_attack{false},
      m_attack_time{data.base_attack_time},
      m_attack_number{0},
      m_number_of_items_spawned{6},
      const_attack_clock{},
      m_clock{}
{
    Ship::s_sprite.setTexture(m_image);
    sf::Vector2u texture_size{m_image.getSize()};
    Ship::s_sprite.setOrigin(texture_size.x / 2, texture_size.y / 2);
    Ship::s_sprite.setPosition(x, y);
    s_health_bar.set_size(300, 30);
    s_health_bar.set_outline_color(sf::Color::Black, 3.f);
    s_health_bar_offset = sf::Vector2f{0, -(texture_size.y / 2.f - 3.f)};
    Ship::toggle_health_bar();
    s_max_health = s_health;
}

void EnemyBoss2::update(Context &context)
{
    Ship::update_health_bar();
    check_health(context);
    constant_attack(context);
    attack(context, m_attack_number);
    cooldown();
}

void EnemyBoss2::attack(Context &context, int attack_number)
{
    // boss information
    sf::Vector2f current_position = s_sprite.getPosition();
    if (m_attack)
    {
        if (attack_number == 0)
        {
            if (clock.getElapsedTime().asSeconds() >= m_attack_time)
            {
                int offset_x = rand() % 3 * 100;
                context.spawn_object(new BasicProjectile{current_position.x + offset_x,
                                                         current_position.y, M_PI_4, 3 * s_projectile_speed, false});
                context.spawn_object(new BasicProjectile{current_position.x - offset_x,
                                                         current_position.y, M_PI_4 + M_PI_2, 3 * s_projectile_speed, false});
                clock.restart();
            }
        }
        if (attack_number == 1)
        {
            if (clock.getElapsedTime().asSeconds() >= m_attack_time)
            {
                int offset_x = (rand() % 2 + 3) * 100;
                context.spawn_object(new BasicProjectile{current_position.x + offset_x,
                                                         0, M_PI_2, 5 * s_projectile_speed, false});
                context.spawn_object(new BasicProjectile{current_position.x - offset_x,
                                                         0, M_PI_2, 5 * s_projectile_speed, false});
                clock.restart();
            }
        }
    }
}

void EnemyBoss2::constant_attack(Context &context)
{
    sf::Vector2f current_position = s_sprite.getPosition();

    float offset_x1 = sin(m_clock.getElapsedTime().asSeconds());
    float offset_x2 = sin(m_clock.getElapsedTime().asSeconds() + M_PI);

    // Constant DNA attack
    if (const_attack_clock.getElapsedTime().asSeconds() >= m_attack_time)
    {
        context.spawn_object(new BasicProjectile{current_position.x + offset_x2 * 200,
                                                 0, M_PI_2, 3 * s_projectile_speed, false});
        context.spawn_object(new BasicProjectile{current_position.x + offset_x1 * 200,
                                                 0, M_PI_2, 3 * s_projectile_speed, false});
        const_attack_clock.restart();
    }
}

void EnemyBoss2::check_health(Context &context)
{
    sf::Vector2f current_position = s_sprite.getPosition();

    if (s_health <= 0)
    {
        GameObject::remove();
        context.get_player()->kills(1000, true);
        // spawn powerups
        for (int i = 0; i < m_number_of_items_spawned; i++)
        {
            int offset_x = i * 50;
            int offset_y = (i - 3) * 50;
            Randomize_powerup(context, current_position.x + offset_x, current_position.x + offset_y);
        }
    }
}

void EnemyBoss2::cooldown()
{
    if (clock.getElapsedTime().asSeconds() >= e_attack_cooldown)
    {
        m_attack = true;
        return;
    }
    m_attack = false;
    m_attack_number = rand() % 2;
}
