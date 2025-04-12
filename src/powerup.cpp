#include "gameobject.hpp"
#include "powerup.hpp"
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include "player.hpp"
#include "projectile.hpp"
#include "resourcemanager.hpp"
#include <cmath>


//Class definition of PowerUp whit initial speed and position.

PowerUp::PowerUp(float x, float y, float v) 
: GameObject { },
activate_nuke{0},
m_sprite     { },
powerup_speed{v},
m_direction  {0.0f, 0.0f},
PowerUp_clock{ }

{
    int rad{9};
    m_sprite.setOrigin(rad, rad);
    m_sprite.setPosition(x, y);
}


void PowerUp::render(sf::RenderWindow &window) const
{
    window.draw(m_sprite);
}

void PowerUp::update(Context &context)
{
    //Here the sprite move downwards and disappears if outside screen or if the
    //countdown exceeds 10 seconds.
    m_sprite.move(0, powerup_speed * context.get_delta().asSeconds());

    if (m_sprite.getPosition().y > context.get_window_size().y)
    {
        remove();
    }
    else if (PowerUp_clock.getElapsedTime().asSeconds() > 10)
    {
        remove();
    }
}

bool PowerUp::handle(const sf::Event &, Context &)
{
    return false;
}

sf::FloatRect PowerUp::bounds() const
{
    return m_sprite.getGlobalBounds();
}

void PowerUp::collision(const GameObject *other)
{
    //If the powerup collides whith the player it desappears and the
    //funktionality is handled whithin the player class. (Except for nuke)
    if (dynamic_cast<const Player *>(other))
	remove();
}



//The separate powerups is defined initialized whith individual sprites.

Repair::Repair(float x, float y, float v)
: PowerUp{x, y, v},
m_image{
    ResourceManager::load_texture("assets/images/powerup_images/Repair_transparent.png")
    }
{
    m_sprite.setTexture(m_image);
}


Speed::Speed(float x, float y, float v)
: PowerUp{x, y, v},
m_image{
    ResourceManager::load_texture("assets/images/powerup_images/Speed_transparent.png")
    }
{
    m_sprite.setTexture(m_image);
}


Buckshot::Buckshot(float x, float y, float v)
: PowerUp{x, y, v},
m_image{
    ResourceManager::load_texture("assets/images/powerup_images/Buckshot_transparent.png")
    }
{
    m_sprite.setTexture(m_image);
}


Boost::Boost(float x, float y, float v)
: PowerUp{x, y, v},
m_image{
    ResourceManager::load_texture("assets/images/powerup_images/Boost_transparent.png")
    }
{    
    m_sprite.setTexture(m_image);
}


Doubleshoot::Doubleshoot(float x, float y, float v)
: PowerUp{x, y, v},
m_image{
    ResourceManager::load_texture("assets/images/powerup_images/Doubleshot_transparent.png")
    }
{
    m_sprite.setTexture(m_image);
}


Add_score::Add_score(float x, float y, float v)
: PowerUp{x, y, v},
m_image{
    ResourceManager::load_texture("assets/images/powerup_images/Add_score.png")
    }
{
    m_sprite.setTexture(m_image);
}


Nuke::Nuke(float x, float y, float v)
: PowerUp{x, y, v},
m_image{
    ResourceManager::load_texture("assets/images/powerup_images/Nuke_transparent.png")
    }
{
    m_sprite.setTexture(m_image);
}

void Nuke::update(Context &context)
{
    //If the player has collided whith a Nuke it fires basic projectiles in
    //every direction.
    if (activate_nuke == 1)
    {
        remove();
        float angle = 0;
        while (angle < 4)        
            {
                context.spawn_object(
                    new BasicProjectile(m_sprite.getPosition().x,
                    m_sprite.getPosition().y,
                    angle*M_PI_2, 500, true));
                angle = angle + 0.05;
                
            }
        activate_nuke = 0;
    }    
    PowerUp::update(context);
}

void Nuke::collision(const GameObject *other)
{
    if (dynamic_cast<const Player *>(other))
        activate_nuke = 1;
}