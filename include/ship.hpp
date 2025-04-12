#pragma once

#include "gameobject.hpp"
#include <SFML/Graphics.hpp>
#include "ui.hpp"

class Ship : public GameObject
{
public:
    Ship(int health, float speed, float projectile_speed);
    virtual ~Ship() = default;
    void render(sf::RenderWindow &window) const override;
    void update(Context &context) override = 0;
    bool handle(const sf::Event &event, Context &context) override = 0;
    sf::FloatRect bounds() const override = 0;
    void collision(const GameObject *other) override = 0;
    int get_health() const;
    int get_max_health() const;

protected:
    // Prefixed with s_ (s for ship) to make it clearer.
    int s_health;
    int s_max_health;
    float s_speed;
    float s_projectile_speed;

    sf::Sprite s_sprite;
    sf::Clock collide_clock;
    sf::Vector2f s_health_bar_offset;
    IntRectangleBar s_health_bar;
    bool s_health_bar_visible;

    /**
     * @brief Toggle the visibility of the health bar. If its visible, it needs
     * to be updated with update_health_bar() separately.
     *
     * @note To customize the health bar, use the s_health_bar member variable and
     * its functions. To change bar offset, set s_health_bar_offset.
     */
    void toggle_health_bar();

    /**
     * @brief Update the health bar position and size. This function should be
     * called every frame if the health bar is visible (called in derived class)
     *
     * @param context[in, out] class containing useful data.
     */
    void update_health_bar();
};