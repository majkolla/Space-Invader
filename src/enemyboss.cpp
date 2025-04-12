#include "enemyboss.hpp"
#include "projectile.hpp"
#include "resourcemanager.hpp"
#include "gameconfiguration.hpp"
#include "player.hpp"
#include "powerup.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>



EnemyBoss::EnemyBoss(const EnemyBossData &data, float x, float y, int)
    : Enemy{data.base_health,
	    0.f,
	    data.base_projectile_speed,
	    data.base_attack_cooldown,
	    0.f,
	    0.f,
	    0.f},
      m_image{ResourceManager::load_texture("assets/images/enemy/boss.png")},
      doing_attack{false},
      attack_nr{0},
      counter{0},
      boss_attack_length{data.base_attack_length},
      attack_time{data.base_attack_time}
{
    clock.restart();
    s_sprite.setTexture(m_image);
    sf::Vector2u texture_size{m_image.getSize()};
    s_sprite.setOrigin(texture_size.x / 2, texture_size.y / 2);
    s_sprite.setPosition(x, y);
    s_health_bar.set_size(300, 30);
    s_health_bar.set_outline_color(sf::Color::Black, 3.f);
    s_health_bar_offset = sf::Vector2f{0, -(texture_size.y / 2.f + 20.f)};
    Ship::toggle_health_bar();
    s_max_health = s_health;
}

void EnemyBoss::update(Context &context)
{
    Ship::update_health_bar();
    if (s_health <= 0)
    {
	spawn_powerups(context);
	context.get_player()->kills(2500, true);
	remove();
    }
    if (doing_attack)
    {
	attack(context);
    }
    else
    {
	if (clock.getElapsedTime().asSeconds() >= e_attack_cooldown)
	{
	    doing_attack = true;
	    attack_nr = rand() % 3;
	    counter = 0;
	    clock.restart();
	}
    }
}

void EnemyBoss::attack(Context &context)
{
    if (attack_nr == 0)
    {
	attack0(context);
    }
    else if (attack_nr == 1)
    {
	attack1(context);
    }
    else if (attack_nr == 2)
    {
	attack2(context);
    }
}

void EnemyBoss::attack0(Context &context)
{
    int width = static_cast<int>(context.get_window_size().x);
    sf::Vector2f cur_pos = s_sprite.getPosition();
    if (clock.getElapsedTime().asSeconds() >= attack_time)
    {
	float random_x{};
	random_x = static_cast<float>(rand()) / RAND_MAX * width;
	context.spawn_object(
	    new BasicProjectile{random_x, cur_pos.y, M_PI_2, s_projectile_speed, false});
	counter++;
	if (counter == boss_attack_length)
	{
	    doing_attack = false;
	}
	clock.restart();
    }
}

void EnemyBoss::attack1(Context &context)
{
    sf::Vector2f cur_pos = s_sprite.getPosition();
    if (clock.getElapsedTime().asSeconds() >= attack_time)
    {
	float random_dir{};
	random_dir = static_cast<float>(rand()) / RAND_MAX;
	context.spawn_object(
	    new BasicProjectile{cur_pos.x, cur_pos.y, static_cast<float>(M_PI) * random_dir, s_projectile_speed, false});
	counter++;
	if (counter == boss_attack_length)
	{
	    doing_attack = false;
	}
	clock.restart();
    }
}

void EnemyBoss::attack2(Context &context)
{
    int width = static_cast<int>(context.get_window_size().x);
    sf::Vector2f cur_pos = s_sprite.getPosition();
    if (clock.getElapsedTime().asSeconds() >= attack_time)
    {
	int x_offset{counter * width / 10};
	context.spawn_object(
	    new BasicProjectile{cur_pos.x + x_offset, cur_pos.y, M_PI_2, s_projectile_speed, false});
	context.spawn_object(
	    new BasicProjectile{cur_pos.x - x_offset, cur_pos.y, M_PI_2, s_projectile_speed, false});
	counter++;
	if (counter == boss_attack_length)
	{
	    doing_attack = false;
	}
	clock.restart();
    }
    
}

void EnemyBoss::spawn_powerups(Context &context)
{
    unsigned int width{context.get_window_size().x};
    sf::Vector2f cur_pos = s_sprite.getPosition();
    float random_x{};
    float rand_val{};
    random_x = static_cast<float>(rand()) / RAND_MAX * width / 2 + width / 4;
    context.spawn_object(new Repair{random_x, cur_pos.y, e_powerup_speed});
    random_x = static_cast<float>(rand()) / RAND_MAX * width / 2 + width / 4;
    rand_val = static_cast<float>(rand()) / RAND_MAX;
    if (rand_val < 0.5)
    {
	context.spawn_object(new Speed{random_x, cur_pos.y, e_powerup_speed});
    }
    else
    {
	context.spawn_object(new Boost{random_x, cur_pos.y, e_powerup_speed});
    }
    random_x = static_cast<float>(rand()) / RAND_MAX * width / 2 + width / 4;
    Randomize_powerup(context, random_x, cur_pos.y);
    rand_val = static_cast<float>(rand()) / RAND_MAX;
    if (rand_val < 0.5)
    {
	random_x = static_cast<float>(rand()) / RAND_MAX * width / 2 + width / 4;
	Randomize_powerup(context, random_x, cur_pos.y);
    }
}
