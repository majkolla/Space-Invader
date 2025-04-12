#include "player.hpp"
#include "projectile.hpp"
#include "enemy.hpp"
#include "powerup.hpp"
#include "resourcemanager.hpp"
#include "gameconfiguration.hpp"
#include "enemyboss.hpp"
#include "enemyboss2.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

Player::Player(const PlayerData &data, float x, float y)
    : Ship{data.health, data.speed, data.projectile_speed},
      m_image{ResourceManager::load_texture("assets/images/rymdskepp/rymdskepp.png")},
      m_shoot_clock{},
      m_powerup_clock{},
      m_score{0},
      m_kills{0},
      m_boss_kills{0},
      m_shoot_speed{data.attack_cooldown},
      m_min_shoot_speed{data.attack_cooldown * 0.25f},
      m_max_speed{data.speed * 4.f},
      m_shot_type{"nothing"},
      m_angle{-M_PI_2},
      m_health_lost{0},
      m_old_pos{}
{
    s_sprite.setTexture(m_image);
    sf::Vector2u texture_size{m_image.getSize()};
    s_sprite.setOrigin(texture_size.x / 2, texture_size.y / 2);
    s_sprite.setPosition(x, y);
    s_health_bar_offset = sf::Vector2f{0, (texture_size.y / 2.f + 10.f)};
    Ship::toggle_health_bar();
    s_max_health = s_health;
}

void Player::update(Context &context)
{
    s_health -= m_health_lost;
    m_health_lost = 0;
    update_health_bar();

    if (s_health <= 0)
    {
        remove();
        return;
    }

    m_old_pos = s_sprite.getPosition();
    sf::Vector2f direction{0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        direction += sf::Vector2f(-1.0f, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        direction += sf::Vector2f(1.0f, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        direction += sf::Vector2f(0.0f, -1.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        direction += sf::Vector2f(0.0f, 1.0f);
    }
    if (std::fabs(direction.x) == 1.0 && std::fabs(direction.y) == 1)
    {
        //if you going diagonal make the direction into the same size as non diagonal
        direction /= static_cast<float>(M_SQRT2);
    }
    if (direction != sf::Vector2f{0.f, 0.f})
    {
        s_sprite.move(direction * s_speed * context.get_delta().asSeconds());
        m_angle = atan2(direction.y, direction.x);
        s_sprite.setRotation(m_angle * 180.f / M_PI + 90.f);
    }

    if (out_of_bounds(context.get_window_size().x, context.get_window_size().y))
    {
        s_sprite.setPosition(m_old_pos);
    }

    if (m_powerup_clock.getElapsedTime().asSeconds() >= 20)
    {
        m_shot_type = "nothing";
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        attack(context);
    }
    if (collide_clock.getElapsedTime().asSeconds() >= 0.5)
    {
        s_sprite.setColor(sf::Color(255, 255, 255));
    }
}

bool Player::handle(const sf::Event &, Context &)
{
    return false;
}

sf::FloatRect Player::bounds() const
{
    return s_sprite.getGlobalBounds();
}

void Player::collision(const GameObject *other)
{
    if (dynamic_cast<const Enemy *>(other))
    {
        if (collide_clock.getElapsedTime().asSeconds() >= 0.5)
        {
            s_health -= 1;
            collide_clock.restart();
            s_sprite.setColor(sf::Color(255, 0, 0));
        }
    }
    if (dynamic_cast<const EnemyBoss2 *>(other))
    {
        s_sprite.setPosition(m_old_pos);
    }
    if (dynamic_cast<const EnemyBoss *>(other))
    {
        s_sprite.setPosition(m_old_pos);
    }
    if (dynamic_cast<const Projectile *>(other))
    {
        const Projectile *current_projectile{dynamic_cast<const Projectile *>(other)};
        // If projectile is not friendly (belongs not to player), player will lose life.
        if (!(current_projectile->is_friendly()))
        {
            s_health -= current_projectile->get_damage();
            s_sprite.setColor(sf::Color(255, 0, 0));
            collide_clock.restart();
        }
    }
    else if (dynamic_cast<const Repair *>(other))
    {
        //if s_health is lower than s_max_health inc s_health else also inc s_max_health
        s_health < s_max_health ? s_health += 1 : s_health = ++s_max_health;
        m_score = m_score + 100;
    }
    if (dynamic_cast<const Speed *>(other))
    {
        if (m_shoot_speed > m_min_shoot_speed)
        {
            m_shoot_speed *= 0.85;
        }
        else
        {
            m_shoot_speed = m_min_shoot_speed;
        }
        m_score = m_score + 100;
    }
    if (dynamic_cast<const Boost *>(other))
    {
        if (s_speed < m_max_speed)
        {
            float prev_speed = s_speed;
            s_speed *= 1.25;
            // Increase projectile speed by the same amount as the ship's speed.
            s_projectile_speed += (s_speed - prev_speed);
        }
        else
        {
            s_speed = m_max_speed;
        }
        m_score = m_score + 100;
    }
    if (dynamic_cast<const Add_score *>(other))
    {
        m_score = m_score + 2000;
    }
    if (dynamic_cast<const Doubleshoot *>(other))
    {
        m_shot_type = "Doubleshoot";
        m_score = m_score + 100;
        m_powerup_clock.restart();
    }
    if (dynamic_cast<const Nuke *>(other))
    {
        m_score = m_score + 200;
    }
    if (dynamic_cast<const Buckshot *>(other))
    {
        m_shot_type = "Buckshot";
        m_score = m_score + 100;
        m_powerup_clock.restart();
    }
}

int Player::get_score() const
{
    return m_score;
}

int Player::get_kills() const
{
    return m_kills;
}

int Player::get_boss_kills() const
{
    return m_boss_kills;
}

//funktion that is made to be called by enemies when the die to giv score
void Player::kills(int score, bool boss) const
{
    m_score = m_score + score;

    if (boss == true)
    {
        m_boss_kills += 1;
    }
    else
    {
        m_kills += 1;
    }
}

// lose_health is a funktion that is used when an enemy pass the screen 
// the health that is send in is the remaining health of the enemy
void Player::lose_health(int health) const
{
    m_health_lost += health;
}

bool Player::out_of_bounds(float width, float height) const
{
    sf::Vector2f pos{s_sprite.getPosition()};
    return pos.x < 0 || pos.x > width || pos.y < 0 || pos.y > height;
}

void Player::attack(Context &context)
{
    if (m_shoot_clock.getElapsedTime().asSeconds() >= m_shoot_speed)
    {
        const sf::Vector2f &pos{s_sprite.getPosition()};
        if (m_shot_type == "Buckshot") 
        {
            //create projectile in ship dirrection and whit an angle of pi/4 the ship direction
            context.spawn_object(new BasicProjectile(
                pos.x, pos.y,m_angle, s_projectile_speed, true));
            context.spawn_object(new BasicProjectile(
                pos.x, pos.y, m_angle - 0.5 * M_PI_2, s_projectile_speed, true));
            context.spawn_object(new BasicProjectile(
                pos.x, pos.y, m_angle + 0.5 * M_PI_2, s_projectile_speed, true));
            m_shoot_clock.restart();
        }
        else if (m_shot_type == "Doubleshoot")
        {
            context.spawn_object(new BasicProjectile(
                pos.x - 5 * sin(m_angle), pos.y - 5 * cos(m_angle), m_angle, s_projectile_speed, true));
            context.spawn_object(new BasicProjectile(
                pos.x + 5 * sin(m_angle), pos.y + 5 * cos(m_angle), m_angle, s_projectile_speed, true));
            m_shoot_clock.restart();
        }
        else
        {
            context.spawn_object(new BasicProjectile(pos.x, pos.y, m_angle, s_projectile_speed, true));
            m_shoot_clock.restart();
        }
    }
}
