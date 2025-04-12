#pragma once

#include <SFML/Graphics.hpp>

#include "context.hpp"
#include "gameconfiguration.hpp"
#include "ui.hpp"

/**
 * @brief A pure virtual class used to define the public API of a GameState.
 */
class GameState
{
public:
    virtual ~GameState() = default;

    /**
     * @brief Render the state. Will be called once every frame.
     *
     * @param window window to draw on.
     */
    virtual void render(sf::RenderWindow &window) const = 0;

    /**
     * @brief Update the state. Will be called once every frame.
     *
     * @param context[in,out] class containing useful data.
     */
    virtual void update(Context &context) = 0;

    /**
     * @brief Let the state handle an event. Will be called once for every event.
     *
     * @param event event to be handled.
     * @param context[in,out] class containing useful data.
     */
    virtual void handle(const sf::Event &event, Context &context) = 0;

    /**
     * @brief Will be called once after the state is switched to.
     *
     * @details Give the derived GameState a chance to initialize its data members.
     *
     * @param gc current game configuration.
     */
    virtual void init(const GameConfiguration &gc) = 0;
};

/*=================================GameMode===================================*/

// Forward declaration
class Player;

/**
 * @brief A pure virtual class, that extend GameSate, defining a GameMode.
 */
class GameMode : public GameState
{
public:
    /**
     * @brief Initialize GameMode with zero game objects.
     */
    GameMode();

    /**
     * @brief Initialize GameMode with given objects.
     *
     * @details GameMode takes ownership of the objects, meaning the objects will
     * be deallocated when GameMode goes out of scope.
     *
     * @param objects vector of all initial game objects.
     */
    GameMode(const std::vector<GameObject *> &objects);

    /**
     * @brief Initialize GameMode with given objects and player. Player should be
     * in the objects vector to be updated and deleted correctly.
     *
     * @details GameMode takes ownership of the objects, meaning the objects will
     * be deallocated when GameMode goes out of scope.
     *
     * @param objects vector of all inital game objects.
     * @param player extra pointer to player for easy access.
     */
    GameMode(const std::vector<GameObject *> &objects, Player *player);

    /**
     * @brief Delete all objects contained in m_objects.
     */
    virtual ~GameMode();

    GameMode(const GameMode &) = delete;
    GameMode &operator=(const GameMode &) = delete;

    /**
     * @brief Will draw background (if set) and call render on all currently 
     * spawned objects.
     * 
     * @param window window to draw on.
     */
    virtual void render(sf::RenderWindow &window) const override;

    /**
     * @brief Will update all currently spawned objects.
     *
     * @details
     * Will do following (in order):
     *      1. update context.
     *      2. check if player is removed (dead), if so end game.
     *      3. call update on all objects.
     *      4. check for collision between object and call collision on objects
     *         that collide.
     *      5. delete all objects that are marked as removed.
     *      6. spawned all new objects contained in context.
     *      7. update music.
     *
     * @param context[in,out] class containing useful data.
     */
    virtual void update(Context &context) override;

    /**
     * @brief Will call handle on objects until the event is consumed. If the event
     * is not consumed all currently spawned objects will handle the given event.
     *
     * @param event event to be handled.
     * @param context[in,out] class containing useful data.
     */
    virtual void handle(const sf::Event &event, Context &context) override;

    /**
     * @brief Will be called once after the state is switched to.
     *
     * @details Give the derived GameMode a chance to initialize its data members
     * based on the current game configuration.
     *
     * @param gc current game configuration.
     */
    virtual void init(const GameConfiguration &gc) override = 0;

    /**
     * @brief Signal that the game will be paused. Pause time will be saved 
     * in m_pause_time & m_paused will be set to true.
     * 
     * @details Gives the derived GameMode a chance to get ready for pause.
     */
    virtual void pause();

protected:
    sf::Clock m_clock;
    sf::Time m_pause_time;
    bool m_paused;

    /**
     * @brief Call update on all objects in m_objects.
     * 
     * @param context[in,out] class containing useful data
     */
    void update_objects(Context &context);

    /**
     * @brief Check for collision between objects in m_objects. If collision is
     * detected call collision on both objects.
     */
    void collision_check();

    /**
     * @brief Delete all objects in m_objects that are marked as removed.
     */
    void delete_removed_objects();

    /**
     * @brief Spawn all new objects contained in context. Will add new objects
     * to m_objects.
     * 
     * @param context[in,out] class containing useful data
     */
    void spawn_new_objects(Context &context);

    /**
     * @brief Set the player. Will delete old player if present. Player will be
     * added to vector of spawned objects.
     *
     * @param player pointer to player.
     */
    void set_player(Player *player);

    /**
     * @brief Get current player. Will return nullptr if no player is present.
     *
     * @return Player* pointer to player.
     */
    Player *get_player() const;

    /**
     * @brief Remove player from vector of spawned objects.
     *
     * @param delete_player true to use delete on player.
     */
    void remove_player(bool delete_player = true);

    /**
     * @brief Add object to vector of spawned objects. Exception will be thrown
     * if object is nullptr.
     *
     * @param object object to be spawned.
     */
    void spawn_object(GameObject *object);

    /**
     * @brief Set a background to the game mode. Background will be scaled to fit
     * the window. Will be drawn before all other objects.
     *
     * @param path path to background image.
     */
    void set_background(const std::string &path, const sf::Vector2u &window_size);

    /**
     * @brief Clear all currently spawned objects.
     *
     * @param delete_player if true, player will also be deleted.
     */
    void clear_objects(bool delete_player = false);

    /**
     * @brief Set the music for the game mode.
     *
     * @param path path to music file.
     * @param volume volume of music. 0.0f - 100.0f.
     */
    void set_music(const std::string &path, float volume = 100.f, bool loop = true);

    /**
     * @brief Set the volume of the music.
     *
     * @param volume volume of music. 0.0f - 100.0f.
     */
    void set_music_volume(float volume);

    /**
     * @brief Fade out the music. Will pause the music when fade is done. Will 
     * not do anything is music is already fading out or fading in.
     *
     * @note Fade out will stop if other music related functions are called.
     *
     * @param duration duration of fade out in seconds.
     */
    void fade_out_music(float duration);

    /**
     * @brief Fade in the music to given target volume. Will start playing music
     * if not already playing. Will not do anything is music is already fading out
     * or fading in.
     *
     * @note Fade in will stop if other music related functions are called.
     *
     * @param duration duration of fade in in seconds.
     * @param target_volume target volume of music. 0.0f - 100.0f.
     */
    void fade_in_music(float duration, float target_volume);

    /**
     * @brief Play the music.
     */
    void play_music();

    /**
     * @brief Pause the music.
     */
    void pause_music();

    /**
     * @brief Set loop of the music.
     *
     * @param loop true to loop music.
     */
    void loop_music(bool loop);

    /**
     * @brief Get the currently spawned objects. Objects will be deleted when
     * GameMode goes out of scope.
     * 
     * @note Primary use is for testing.
     * 
     * @return const std::vector<GameObject *>& currently spawned objects.
     */
    const std::vector<GameObject *> &get_objects() const;

    /**
     * @brief Get the music. Music will be deleted when GameMode goes out of
     * scope.
     * 
     * @note Primary use is for testing.
     * 
     * @return const sf::Music& music.
     */
    sf::Music &get_music();

private:
    /**
     * @brief Enum to keep track of the state of the music.
     */
    enum class FadeType
    {
        None,
        FadeIn,
        FadeOut
    };

    std::vector<GameObject *> m_objects;
    Player *m_player;

    sf::Sprite m_background;
    sf::Music m_music;

    // Music data needed for fading the music.
    float m_music_volume;
    sf::Time m_music_fade_start;
    float m_music_fade_duration;
    FadeType m_music_fade_type;

    /**
     * @brief Update the current music. Will be called every frame, but only
     * do something if music is currently fading.
     */
    void update_music();
};

/*====================================MENU====================================*/

/**
 * @brief A pure virtual class, that extend GameState, defining a menu.
 */
class Menu : public GameState
{
public:
    /**
     * @brief Initialize a Menu with zero buttons.
     */
    Menu();

    /**
     * @brief Delete all buttons & sprites.
     */
    virtual ~Menu();

    Menu(const Menu &) = delete;
    Menu &operator=(const Menu &) = delete;

    /**
     * @brief Render the Menu. Will call render on all added buttons and sprites.
     * Buttons will be rendered on top of sprites, meaning sprites are rendered
     * first.
     *
     * @param window window to draw on.
     */
    virtual void render(sf::RenderWindow &window) const override;

    /**
     * @brief Update the Menu. Will call update on all added buttons.
     *
     * @param context[in,out]  class containing useful data.
     */
    virtual void update(Context &context) override;

    /**
     * @brief Handle the Menu. Will call handle_click on all added buttons.
     *
     * @param event event to be handled.
     * @param context context[in,out]  class containing useful data.
     */
    virtual void handle(const sf::Event &event, Context &context) override;

    /**
     * @brief Will be called once after the state is switched to.
     *
     * @details Give the derived Menu a chance to initialize its data members.
     *
     * @param gc current game configuration.
     */
    virtual void init(const GameConfiguration &gc) override = 0;

protected:
    std::vector<Button *> m_buttons;
    std::vector<sf::Sprite *> m_sprites;

    /**
     * @brief Add a button to the menu. Note that the menu takes ownership of
     * the added button.
     *
     * @param button button to be added.
     */
    void add_button(Button *button);

    /**
     * @brief Add a sprite to the menu. Note that menu takes ownership of the
     * added sprite. Added sprite will be drawn once every frame.
     *
     * @param sprite sprite to be added.
     */
    void add_sprite(sf::Sprite *sprite);
};