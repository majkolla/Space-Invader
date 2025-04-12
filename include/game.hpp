#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "gamestate.hpp"

/**
 * @brief Class responsible for the games main-loop and creating a window.
 */
class Game
{
public:
    /**
     * @brief Initialize a game object with default values. Will be initalized
     * with no game state.
     */
    Game();

    /**
     * @brief Initialize a game object. Will create a sf::RenderWindow with given
     * title, width and height. Will be initalized with no GameState.
     *
     * @param title title of window.
     * @param width window width.
     * @param height window height
     */
    Game(std::string title, unsigned int width, unsigned int height);

    /**
     * @brief Initialize a game object. Will create a sf::RenderWindow based on
     * the given GameConfiguration. Will be initalized with the given GameState.
     * 
     * @param gc game configuration.
     * @param start first GameState.
     */
    Game(const GameConfiguration& gc, GameState* start);

    /**
     * @brief Initialize a game object. Will create a sf::RenderWindow with given
     * title, width and height. Will be initalized with the given GameState.
     *
     * @param title title of window.
     * @param width window width
     * @param height window height
     * @param start first GameState.
     */
    Game(std::string title, unsigned int width, unsigned int height, GameState *start);

    /**
     * @brief Destroy the Game object. Deletes the current state.
     */
    ~Game();

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    /**
     * @brief Start the games main loop. Will throw an exception if no GameState
     * is set.
     */
    void run();

    /**
     * @brief Set the games current state. The old state, if not null, will be
     * deleted.
     *
     * @param new_state
     */
    void set_state(GameState *new_state);

private:
    sf::RenderWindow m_window;
    GameState *m_current_state;
    GameConfiguration m_configuration;

    /**
     * @brief Handle global game events. Called once for every polled event.
     *
     * @param event event to be handled.
     */
    void handle(const sf::Event &event);

    /**
     * @brief Handles the context. Will be called once every frame.
     *
     * @param context[in,out] class containing useful data.
     */
    void handle_context(Context &context);

    /**
     * @brief Switch the current state to the given state.
     *
     * @param state state to switch to.
     */
    void switch_state(GameState *state);
};

