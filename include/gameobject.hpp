#pragma once

#include <SFML/Graphics.hpp>

#include "context.hpp"

/**
 * @brief Pure virtual class defining public API of a GameObject.
 */
class GameObject
{
public:
    GameObject();
    virtual ~GameObject() = default;

    /**
     * @brief Render the GameObject. Will be called once every frame.
     *
     * @param window window to draw on.
     */
    virtual void render(sf::RenderWindow &window) const = 0;

    /**
     * @brief Update the GameObject. Will be called once every frame.
     *
     * @param context[in,out]  class containing useful data.
     */
    virtual void update(Context &context) = 0;

    /**
     * @brief Let the GameObject handle a given event. Not guaranteed to be called
     * for every event.
     *
     * @param event event to be handled.
     * @param context[in,out]  class containing useful data.
     * @return return true to block this event, meaning no additional GameObjects
     * can handle this event.
     * @return return false to let other GameObjects handle this event.
     */
    virtual bool handle(const sf::Event &event, Context &context) = 0;

    /**
     * @brief Returns the objects bounds. Will be used for collision detection.
     *
     * @return sf::FloatRect objects hit box.
     */
    virtual sf::FloatRect bounds() const  = 0;

    /**
     * @brief Will be called once for every collision.
     *
     * @param other object that was collided with.
     */
    virtual void collision(const GameObject *other) = 0;

    /**
     * @brief Check collision between self and other. Uses the bounds method
     * to get hit box (sf::FloatRect).
     *
     * @param other
     * @return true
     * @return false
     */
    bool collides(const GameObject *other) const;

    /**
     * @brief Used to mark object for deletion. 
     */
    void remove();

    /**
     * @brief Check if object is removed.
     * 
     * @return true 
     * @return false 
     */
    bool is_removed() const;

protected:
    bool m_removed;
};
