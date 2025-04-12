#include "ship.hpp"
#include <cmath>

Ship::Ship(int health, float speed, float projectile_speed)
    : GameObject{},
      s_health{health},
      s_max_health{health},
      s_speed{speed},
      s_projectile_speed{projectile_speed},
      s_sprite{},
      collide_clock{},
      s_health_bar_offset{},
      s_health_bar{s_max_health, s_health},
      s_health_bar_visible{false}
{
    s_health_bar_offset = sf::Vector2f{0, 34};
    s_health_bar.set_size(70, 6);
    s_health_bar.set_bar_color(sf::Color::Cyan);
    s_health_bar.set_background_color(sf::Color{0x4b4b4bff});
}

void Ship::render(sf::RenderWindow &window) const
{
    window.draw(s_sprite);
    if (s_health_bar_visible)
    {
        s_health_bar.render(window);
    }
}

int Ship::get_health() const
{
    return s_health;
}

int Ship::get_max_health() const
{
    return s_max_health;
}

void Ship::toggle_health_bar()
{
    s_health_bar_visible = !s_health_bar_visible;
}

void Ship::update_health_bar()
{
    sf::Vector2f pos = s_sprite.getPosition() + s_health_bar_offset;
    ;
    s_health_bar.set_position(pos.x, pos.y);
    s_health_bar.update();
}