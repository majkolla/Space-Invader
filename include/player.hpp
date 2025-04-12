#pragma once

#include "ship.hpp"
#include <SFML/Graphics.hpp>
#include <string>

struct PlayerData;

class Player : public Ship
{
public:
    Player(const PlayerData &data, float x, float y);
    virtual ~Player() = default;
    void update(Context &context) override;
    bool handle(const sf::Event &event, Context &context) override;
    sf::FloatRect bounds() const override;
    void collision(const GameObject *other) override;
    void kills(int score, bool boss) const;
    int get_score() const;
    int get_kills() const;
    int get_boss_kills() const;
    void lose_health(int health = 1) const;

private:
    const sf::Texture &m_image;
    sf::Clock m_shoot_clock;
    sf::Clock m_powerup_clock;
    mutable int m_score;
    mutable int m_kills;
    mutable int m_boss_kills;
    float m_shoot_speed;
    float m_min_shoot_speed;
    float m_max_speed;
    std::string m_shot_type;
    float m_angle;
    mutable int m_health_lost;
    sf::Vector2f m_old_pos; // Store the old position of the player every frame

    bool out_of_bounds(float width, float height) const;
    void attack(Context &context);
};
