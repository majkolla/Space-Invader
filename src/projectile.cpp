#include "projectile.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include <cmath>

Projectile::Projectile(float x, float y, float angle, float v, bool friendly, int damage)
    : GameObject{}, m_circle{3}, m_direction{std::cos(angle), std::sin(angle)}, m_speed{v}, friendly{friendly}, damage{damage}
{
    float radius{3};
    m_circle.setOrigin(radius, radius);
    m_circle.setPosition(x, y);

    uint8_t r = 0;
    uint8_t g = 255;
    uint8_t b = 0;
    m_circle.setFillColor(sf::Color{r, g, b});
}

void Projectile::render(sf::RenderWindow &window) const
{
    window.draw(m_circle);
}

void Projectile::update(Context &context)
{
    m_circle.move(
        m_speed * m_direction * context.get_delta().asSeconds());

    sf::Vector2f pos = m_circle.getPosition();
    // Check if out of bounds.
    if (pos.y > context.get_window_size().y + 10.0f || pos.y < -10.0f || pos.x < -10.0 
	|| pos.x > context.get_window_size().x + 10.0f)
    {
        remove();
    }
}

bool Projectile::handle(const sf::Event &, Context &)
{
    return false;
}

sf::FloatRect Projectile::bounds() const
{
    return m_circle.getGlobalBounds();
}
bool Projectile::is_friendly() const
{
    return friendly;
}

int Projectile::get_damage() const
{
    return damage;
}

BasicProjectile::BasicProjectile(float x, float y, float angle, float v, bool friendly, int damage)
    : Projectile::Projectile(x, y, angle, v, friendly, damage)
{
}

void BasicProjectile::collision(const GameObject *other)
{
    if (dynamic_cast<const Enemy *>(other) && friendly)
    {
        remove();
    }
    if (dynamic_cast<const Player *>(other) && !friendly)
    {
        remove();
    }
}
