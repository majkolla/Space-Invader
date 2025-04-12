#pragma once

#include "gamestate.hpp"
#include "ui.hpp"

class MainMenu : public Menu
{
public:
    MainMenu();
    virtual ~MainMenu();

    /**
     * @brief Render the menu. Will be called once every frame. Also calls
     * Menu render function.
     *
     * @param window window to draw on.
     */
    void render(sf::RenderWindow &window) const override;

    /**
     * @brief Update the menu. Will be called once every frame. Also calls Menu
     * update function.
     *
     * @param context[in,out] class containing useful data.
     */
    void update(Context &context) override;

    /**
     * @brief Handle events. Also calls Menu handle function.
     *
     * @param event event to be handled.
     * @param context[in,out] class containing useful data.
     */
    void handle(const sf::Event &event, Context &context) override;

    /**
     * @brief Initialize the game mode based on the current game configuration.
     *
     * @details This function should be called after the game mode is created.
     * Will initialize all the game objects and other game mode specific data,
     * such as the background, texts and timers.
     *
     * @note If this function is not called, the game mode will not function properly.
     *
     * @param gc game configuration.
     */
    void init(const GameConfiguration &gc) override;

    /**
     * @brief Toggle help screen.
     *
     * @details Is needed to be public because it is used by a button.
     */
    void toggle_help();

private:
    bool m_show_help;
    sf::Sprite m_help_sprite;
};