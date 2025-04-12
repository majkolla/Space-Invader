#pragma once

#include "gamestate.hpp"
#include "gameobject.hpp"

class NormalMode : public GameMode
{
public:
    NormalMode();
    virtual ~NormalMode();

    /**
     * @brief Render the state. Will be called once every frame. Also calls
     * GameMode render function.
     *
     * @param window window to draw on.
     */
    void render(sf::RenderWindow &window) const override;

    /**
     * @brief Update the state. Will be called once every frame. Also calls GameMode
     * update function.
     *
     * @param context[in,out] class containing useful data.
     */
    void update(Context &context) override;

    /**
     * @brief Handle events. Also calls GameMode handle function.
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

private:
    // Spawn related data.
    float m_spawn_time, m_spawn_time_multiplier, m_spawn_time_min;
    float m_boss_spawn_time, m_current_boss_time;
    sf::Time m_prev_spawn_time;
    sf::Time m_prev_boss_spawn_time;
    unsigned int m_boss_counter;

    unsigned int m_spawn_zone;
    unsigned int m_spawn_zones;
    unsigned int m_spawn_seed;

    // Difficulty related data.
    unsigned int m_level_rating;
    float m_level_inc_rate, m_current_level_time;
    sf::Time m_level_time;
    sf::Sound m_level_up_sound;

    // UI related data.
    sf::Sprite m_scoreboard;
    sf::Text m_level_number_text;
    sf::Text m_player_score_text;
    sf::Text m_player_health_text;
    sf::Text m_boss_countdown_text;
    sf::RectangleShape m_boss_warning_rect;
    FloatRectangleBar m_level_bar;
    FloatRectangleBar m_boss_countdown_bar;

    EnemyMinionData m_minion_data;

    /**
     * @brief Render all texts.
     *
     * @param window window to draw to.
     */
    void render_texts(sf::RenderWindow &window) const;

    /**
     * @brief Render all bars.
     *
     * @param window window to draw to.
     */
    void render_bars(sf::RenderWindow &window) const;

    /**
     * @brief Update all texts.
     */
    void update_texts();

    /**
     * @brief Handle time related events.
     *
     * @param context[in,out] class containing useful data.
     */
    void handle_time(Context &context);

    /**
     * @brief Initialize all texts.
     *
     * @param gc game configuration.
     */
    void init_texts(const GameConfiguration &gc);

    /**
     * @brief Initialize all bars.
     *
     * @param gc game configuration.
     */
    void init_bars(const GameConfiguration &gc);

    /**
     * @brief Initialize UI. Will call init_texts and init_bars functions.
     *
     * @param gc game configuration.
     */
    void init_ui(const GameConfiguration &gc);

    /**
     * @brief Initialize all data members based on loaded game configuration.
     *
     * @param data NormalModeData struct containing all data members.
     */
    void init_data(const NormalModeData &data);

    /**
     * @brief Get an enemy. Based on the current level, will return a random enemy.
     *
     * @details The position of the enemy will be random, but will be based on the
     * current spawn zone.
     *
     * Enemy spawned will be based on the current level. The higher the level, the
     * more difficult the enemies will be. Difficulty will increase up to a
     * certain point.
     *
     * @return GameObject* enemy pointer.
     */
    GameObject *get_enemy(const sf::Vector2u &window_size);

    /**
     * @brief Get a random position on the screen, based on the current spawn zone
     * and the screen size.
     *
     * @param window_size size of current window.
     * @return sf::Vector2f position where to spawn the object.
     */
    sf::Vector2f get_enemy_position(const sf::Vector2u &window_size);

    /**
     * @brief Get the random enemy. Used in get_enemy function.
     *
     * @param pos enemy position.
     * @return GameObject* enemy pointer.
     */
    GameObject *get_enemy_first(const sf::Vector2f &pos) const;

    /**
     * @brief Get the random enemy.Used in get_enemy function.
     *
     * @param pos enemy position.
     * @return GameObject* enemy pointer.
     */
    GameObject *get_enemy_second(const sf::Vector2f &pos) const;

    /**
     * @brief Get the random enemy. Used in get_enemy function.
     *
     * @param pos enemy position.
     * @return GameObject* enemy pointer.
     */
    GameObject *get_enemy_third(const sf::Vector2f &pos) const;

    /**
     * @brief Get the random enemy. Used in get_enemy function.
     *
     * @param pos enemy position.
     * @return GameObject* enemy pointer.
     */
    GameObject *get_enemy_fourth(const sf::Vector2f &pos) const;

    /**
     * @brief Get the random enemy. Used in get_enemy function.
     *
     * @param pos enemy position.
     * @return GameObject* enemy pointer.
     */
    GameObject *get_enemy_default(const sf::Vector2f &pos) const;

    /**
     * @brief Go to boss state. Context needed to set next state.
     *
     * @param context[in,out] class containing useful data.
     */
    void to_boss(Context &context);
};