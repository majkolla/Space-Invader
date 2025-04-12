#pragma once
#include "gamestate.hpp"

class PauseMenu : public Menu
{
public:
    PauseMenu(GameState *previous_state);
    virtual ~PauseMenu();

    PauseMenu(const PauseMenu &) = delete;
    PauseMenu &operator=(const PauseMenu &) = delete;

    /**
     * @brief Render PauseMenu. Will be called once every frame. Also calls Menu
     * render function. Will render the previous state if it exists.
     * 
     * @param window window to draw on.
     */
    virtual void render(sf::RenderWindow &window) const override;

    /**
     * @brief Update PauseMenu. Will be called once every frame. Also calls Menu
     * update function.
     * 
     * @param context[in,out] class containing useful data.
     */
    virtual void update(Context &context) override;

    /**
     * @brief Handle events. Also calls Menu handle function.
     * 
     * @param event event to be handled.
     * @param context[in,out] class containing useful data.
     */
    virtual void handle(const sf::Event &event, Context &context) override;

    /**
     * @brief Initialize the game mode based on the current game configuration.
     * 
     * @param gc game configuration.
     */
    virtual void init(const GameConfiguration &gc) override;

    /**
     * @brief Unpause the game.
     * 
     * @details Is needed to be public because it is used by a button.
     * 
     * @param context[in,out] class containing useful data.
     */
    void unpause(Context &context);

private:
    GameState *m_previous_state;
};