#pragma once

#include "gameobject.hpp"
#include <SFML/Graphics.hpp>
#include  "context.hpp"
#include "resourcemanager.hpp"


//Class definition of PowerUp as an inheritance class with basic funktionality
//as moving, collision and so on.

class PowerUp: public GameObject
{
public:
    PowerUp(float x, float y, float v);
    ~PowerUp() =default;
    void render(sf::RenderWindow &window) const;
    void update(Context &context);
    bool handle(const sf::Event &event, Context &context);
    sf::FloatRect bounds() const;
    virtual void collision(const GameObject *other);
    bool activate_nuke;
    sf::Sprite m_sprite;
private:
    float powerup_speed;
    sf::Vector2f m_direction;
    sf::Clock PowerUp_clock;
};


//Class definition of all powerups with individual textures. Funktionality is 
//in the player file exept for the class "Nuke".

class Repair: public PowerUp
{
public:
    Repair(float x, float y, float v);
private:
    const sf::Texture &m_image;
};


class Speed: public PowerUp
{
public:
    Speed(float x, float y, float v);
private:
    const sf::Texture &m_image;
};


class Buckshot: public PowerUp
{
public:
    Buckshot(float x, float y, float v);
private:
    const sf::Texture &m_image;
};


class Boost: public PowerUp
{
public:
    Boost(float x, float y, float v);
private:
    const sf::Texture &m_image;
};


class Doubleshoot: public PowerUp
{
public:
    Doubleshoot(float x, float y, float v);
private:
    const sf::Texture &m_image;
};


class Add_score: public PowerUp
{
public:
    Add_score(float x, float y, float v);
private:
    const sf::Texture &m_image;
};


class Nuke: public PowerUp
{
public:
    Nuke(float x, float y, float v);
    void update(Context &context) override;
    void collision(const GameObject *other);
private:
    const sf::Texture &m_image;
};