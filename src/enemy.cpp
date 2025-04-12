#include "enemy.hpp"
#include "projectile.hpp"
#include "player.hpp"
#include "resourcemanager.hpp"
#include "gameconfiguration.hpp"
#include "powerup.hpp"

Enemy::Enemy(
    int health,
    float speed,
    float projectile_speed,
    float projectile_time,
    float projectile_prob,
    float powerup_prob,
    float powerup_speed,
    const sf::Vector2f &move_direction)
    : Ship{health, speed, projectile_speed},
      clock{},
      e_powerup_prob{powerup_prob},
      e_attack_cooldown{projectile_time},
      e_projectile_prob{projectile_prob},
      e_powerup_speed{powerup_speed},
      e_move_direction{move_direction}
{
}

void Enemy::update(Context &context)
{
    if (clock.getElapsedTime().asSeconds() >= e_attack_cooldown)
    {
        // At this time only probabilities with 2 decimal places are supported.
        int rnd_time_projectile = rand() % 100;
        if (rnd_time_projectile >= (1.f - e_projectile_prob) * 100.f)
        {
            attack(context);
        }
        clock.restart();
    }

    if (s_health <= 0)
    {
        // At this time only probabilities with 2 decimal places are supported.
        int rnd_time_powerup = rand() % 100;
        if (rnd_time_powerup >= (1 - e_powerup_prob) * 100.f)
        {
            Randomize_powerup(context, s_sprite.getPosition().x, s_sprite.getPosition().y);
        }
        context.get_player()->kills(100, false);
        remove();
    }

    s_sprite.move(e_move_direction * s_speed * context.get_delta().asSeconds());

    // If the minion is outside the screen in the y-direction, it will be removed.
    if (s_sprite.getPosition().y > context.get_window_size().y)
    {
        context.get_player()->lose_health();
        remove();
    }
}

bool Enemy::handle(const sf::Event &, Context &)
{
    return false;
}

sf::FloatRect Enemy::bounds() const
{
    return s_sprite.getGlobalBounds();
}

void Enemy::collision(const GameObject *other)
{
    // If enemy collides with player, it will lose one life.
    if (dynamic_cast<const Player *>(other))
    {
        if (collide_clock.getElapsedTime().asSeconds() >= 0.5)
        {
            s_health -= 1;
            collide_clock.restart();
        }
    }
    if (dynamic_cast<const Projectile *>(other))
    {
        const Projectile *current_projectile{dynamic_cast<const Projectile *>(other)};
        // If projectile is friendly (belongs to player), the enemy will lose life.
        if (current_projectile->is_friendly())
        {
            s_health -= current_projectile->get_damage();
        }
    }
}

void Enemy::attack(Context &)
{
    return;
}

void Enemy::Randomize_powerup(Context &context, float x_pos, float y_pos) const
{
    int random_powerup = (rand() % 70) + 1;
    if (random_powerup < 11)
    {
        context.spawn_object(new Repair{x_pos, y_pos, e_powerup_speed});
    }
    else if (10 < random_powerup and random_powerup < 21)
    {
        context.spawn_object(new Speed{x_pos, y_pos, e_powerup_speed});
    }
    else if (20 < random_powerup and random_powerup < 31)
    {
        context.spawn_object(new Buckshot{x_pos, y_pos, e_powerup_speed});
    }
    else if (30 < random_powerup and random_powerup < 41)
    {
        context.spawn_object(new Doubleshoot{x_pos, y_pos, e_powerup_speed});
    }
    else if (40 < random_powerup and random_powerup < 51)
    {
        context.spawn_object(new Boost{x_pos, y_pos, e_powerup_speed});
    }
    else if (50 < random_powerup and random_powerup < 61)
    {
        context.spawn_object(new Add_score{x_pos, y_pos, e_powerup_speed});
    }
    else
    {
        context.spawn_object(new Nuke{x_pos, y_pos, e_powerup_speed});
    }
}