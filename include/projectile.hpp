#pragma once

#include <SFML/Graphics.hpp>

#include "context.hpp"

#include "gameobject.hpp"
 
/**
 * Class representing a projectile. 
 * All projectiles use arguments 
 * float x, float y, float angle ,float v, bool friendly.
 *
 * x and y are coordinates to the projectiles starting posisition
 * angle is the prjoectiles trejectory in radians, use M_PI / 2 to get
 * trejotory straight down and 3 * M_PI / 2 to get trejectory straight up
 * v is the projectiles initailal speed.
 * friendly is indicating if the projectile was fired by player or by an enemy
 * If fierd by player friendly should be set to true and if fiered by an enemy
 * friendly should be set to false.
 * damage is the damage done by the projectile, deafault value 1
 */

class Projectile : public GameObject
{
public:
    Projectile(float x, float y, float angle ,float v, bool friendly, int damage = 1);
    ~Projectile() = default;
    void render(sf::RenderWindow &window) const override;
    void update(Context &context) override;
    bool handle(const sf::Event &event, Context &context) override;
    sf::FloatRect bounds() const override;
    bool is_friendly() const;
    int get_damage() const;

protected:
    sf::CircleShape m_circle;
    sf::Vector2f m_direction;
    float m_speed;
    bool friendly;
    int damage;
};

class BasicProjectile : public Projectile
{
public:
    BasicProjectile(float x, float y, float angle ,float v, bool friendly, int damage = 1);
    ~BasicProjectile() = default;
    void collision(const GameObject *other) override;
};
