#pragma once
#include "ship.hpp"
#include <SFML/Graphics.hpp>

struct EnemyMinionData;
struct EnemyBossData;

class Enemy : public Ship
{
public:
    Enemy(int health,
          float speed,
          float projectile_speed,
          float attack_cooldown,
          float projectile_prob,
          float powerup_prob,
          float powerup_speed,
          const sf::Vector2f &move_direction = {0.f, 1.0f});
    ~Enemy() = default;

    /**
     * @brief Update function, with common functionality for all enemies.
     *
     * @details Will do following:
     *          1. Check if the enemy is ready to attack. If so, call attack function.
     *          2. Check if the enemy is dead. If so, remove it from the game.
     *          3. Move the enemy.
     *          4. Check if the enemy is outside the screen. If so, remove it from the game.'
     *             Only checks the y position (outside bottom of the screen).
     *
     * @param context[in,out] class containing useful data.
     */
    void update(Context &context) override;
    bool handle(const sf::Event &event, Context &context) override;
    sf::FloatRect bounds() const override;
    void collision(const GameObject *other) override;

protected:
    sf::Clock clock;
    // Prefixed with e_ (e for enemy) to make it clearer.
    float e_powerup_prob;
    float e_attack_cooldown;
    float e_projectile_prob;
    float e_powerup_speed;
    sf::Vector2f e_move_direction;

    void Randomize_powerup(Context &context, float x_pos, float y_pos) const;

    /**
     * @brief Attack function for the enemy. Will be called when the enemy is ready to attack,
     * if Enemy update function is used. Default implementation will do nothing.
     *
     * @param context[in,out] class containing useful data.
     */
    virtual void attack(Context &context);
};