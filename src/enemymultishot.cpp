#include "enemymultishot.hpp"
#include "projectile.hpp"
#include "resourcemanager.hpp"
#include "gameconfiguration.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

EnemyMultiShot::EnemyMultiShot(const EnemyMinionData &data, float x, float y, int)
    : Enemy{data.base_health,
            data.base_speed,
            data.base_projectile_speed,
            data.base_attack_cooldown * 2.0f,
            data.base_projectile_prob,
            data.base_powerup_prob * 2.0f,
            data.base_speed * 0.5f},
      m_image{ResourceManager::load_texture("assets/images/enemy/enemy.png")}
{
    clock.restart();
    s_sprite.setTexture(m_image);
    sf::Vector2u texture_size{m_image.getSize()};
    s_sprite.setOrigin(texture_size.x / 2, texture_size.y / 2);
    s_sprite.setPosition(x, y);
    s_health_bar_offset = sf::Vector2f{0, (texture_size.y / 2.f + 10.f)};
    Ship::toggle_health_bar();

}

void EnemyMultiShot::update(Context &context)
{
    Ship::update_health_bar();
    Enemy::update(context);
}

void EnemyMultiShot::attack(Context &context)
{
    sf::Vector2f cur_pos = s_sprite.getPosition();
    context.spawn_object(new BasicProjectile{cur_pos.x, cur_pos.y, 0, s_projectile_speed, false});
    context.spawn_object(new BasicProjectile{cur_pos.x, cur_pos.y, 2 * M_PI_2, s_projectile_speed, false});
    context.spawn_object(new BasicProjectile{cur_pos.x, cur_pos.y, 3 * M_PI_2, s_projectile_speed, false});
    context.spawn_object(new BasicProjectile{cur_pos.x, cur_pos.y, M_PI_2, s_projectile_speed, false});
}
