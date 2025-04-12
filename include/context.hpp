#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

// Forward declaration
class GameObject;
class GameState;
class Player;
typedef std::vector<GameObject *> ObjectVector;

/**
 * @brief Contains all data needed by GameStates and GameObjects. Will be
 * used to pass data throughout the program.
 *
 * @attention objects passed into Context must live as long or longer than the
 * created context object. Otherwise you can get access to deallocated memory.
 */
class Context
{
public:
    /**
     * @brief Initialize Context with given delta time and window size.
     *
     * @param delta time between current and last frame.
     * @param window current window..
     */
    Context(const sf::Time &delta, const sf::RenderWindow &window);

    /**
     * @brief Will throw exception if m_new_objects is not empty.
     *
     * @details A non empty m_new_objects vector means that there is memory leaks.
     * Exception used for debugging, might be removed later.
     */
    ~Context() noexcept(false);

    Context(const Context &) = delete;
    Context &operator=(Context &) = delete;

    /**
     * @brief Set delta.
     *
     * @param delta time between current and last frame.
     */
    void set_delta(const sf::Time &delta);

    /**
     * @brief Set next game state. If next state is already set, it will not
     * be set.
     *
     * @param next_state next game state.
     * @return true if state is not set.
     * @return false if state is already set.
     */
    bool set_next_state(GameState *next_state);

    /**
     * @brief Add GameObjects to the spawn queue. Takes ownership of the spawned
     * GameObject.
     *
     * @param game_object GameObject to be spawned.
     */
    void spawn_object(GameObject *object);

    /**
     * @brief Set the current player.
     *
     * @param player current player.
     */
    void set_player(const Player *player);

    /**
     * @brief Get delta-time between current and last frame.
     *
     * @return sf::Time
     */
    const sf::Time &get_delta() const;

    /**
     * @brief Get next game state. Nullptr if no next state is set.
     *
     * @return GameState*
     */
    GameState *get_next_state() const;

    /**
     * @brief Get all new objects that should be spawned. All objects will be
     * given away, meaning the caller is now the owner of all new objects.
     *
     * @details The objects are given away to prevent multiple pointers to
     * the same object accidentally. Which can lead to undefined behaviour.
     *
     * @param objects[out] the vector with new objects will be moved into this
     * parameter.
     */
    void get_new_objects(std::vector<GameObject *> &objects);

    /**
     * @brief Get the window size as sf::Vector2u.
     *
     * @return sf::Vector2u window size.
     */
    sf::Vector2u get_window_size() const;

    /**
     * @brief Get a const reference to current window.
     *
     * @return const sf::RenderWindow&
     */
    const sf::RenderWindow &get_window() const;

    /**
     * @brief Get read only reference to current player. Will throw exception
     * if player is not set.
     *
     * @return const Player* current player.
     */
    const Player *get_player() const;

    /**
     * @brief Exit the game.
     */
    void exit();

    /**
     * @brief Check if exit() has been called.
     *
     * @return true if exit() has been called.
     */
    bool has_exited() const;

private:
    sf::Time m_delta;
    GameState *m_next_state;
    std::vector<GameObject *> m_new_objects;
    const sf::RenderWindow &m_window;
    bool m_quit;
    const Player *m_player;
    /* Should contain everything that objects and states need*/
};