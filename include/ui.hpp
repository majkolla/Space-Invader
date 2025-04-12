#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

#include "context.hpp"

/**
 * @brief Pure virtual class defining public API for a static UI element;
 */
class UI
{
public:
    virtual ~UI() = default;

    /**
     * @brief Render UI element. Will be called once every frame;
     *
     * @param window window to draw on.
     */
    virtual void render(sf::RenderWindow &window) const = 0;
};

/**
 * @brief Pure virtual class defining public API for a dynamic UI element.
 */
class DynamicUI : public UI
{
public:
    virtual ~DynamicUI() = default;
    /**
     * @brief Render UI element. Will be called once every frame.
     *
     * @param window window to draw on.
     */
    virtual void render(sf::RenderWindow &window) const = 0;

    /**
     * @brief Update UI element. Will be called once every frame.
     *
     * @param context context[in,out]  class containing useful data.
     */
    virtual void update(Context &context) = 0;
};

/*=====================================Button=================================*/

enum class ButtonSoundConfig
{
    NONE,
    HOVER,
    CLICK,
    HOVER_AND_CLICK
};

/**
 * @brief Basic hoverable and clickable button.
 *
 * @details Button can be hovered over and clicked. If button is hovered over,
 * it will change texture to hover texture. If button is clicked, it will call
 * a function with a pointer to the game state as an argument. The argument is
 * needed when you want to change the game state from within the button function.
 * If you do not need to change the game state, you can pass nullptr as the argument.
 *
 * All buttons have default click and hover sounds. To change sound config,
 * pass desired config to constructor:
 *      ButtonSoundConfig::NONE  - no sounds.
 *      ButtonSoundConfig::HOVER - hover sound only.
 *      ButtonSoundConfig::CLICK - click sound only.
 *      ButtonSoundConfig::HOVER_AND_CLICK - both hover and click sounds.
 *
 * @note If you want to change the default sounds, you can do so by using appropriate
 * functions. See set_click_sound(...), set_hover_sound(...) and set_sound(...).
 */
class Button : public DynamicUI
{
public:
    /**
     * @brief Construct a Button with position (x, y) and with the given image.
     * Will throw exception if image is not loaded properly.
     *
     * @param x pixel position.
     * @param y pixel position.
     * @param image path to image.
     * @param sound_config which sounds to play
     */
    Button(
        int x,
        int y,
        const std::string &image,
        ButtonSoundConfig sound_config = ButtonSoundConfig::NONE);

    /**
     * @brief Construct a new Button with given position, image and click function.
     * Will throw exception if image is not loaded properly.
     *
     * @param x pixel position.
     * @param y pixel position.
     * @param image path to image.
     * @param func ptr to function to be called when clicked.
     * @param sound_config which sounds to play
     * @param state_arg ptr to state to be passed to function.
     */
    Button(
        int x,
        int y,
        const std::string &image,
        void (*func)(Context &, GameState *),
        ButtonSoundConfig sound_config = ButtonSoundConfig::NONE,
        GameState *state_arg = nullptr);

    /**
     * @brief Construct a new Button with given position, image and hover image.
     * Will throw exception if images are not loaded properly.
     *
     * @param x pixel position.
     * @param y pixel position.
     * @param image path to image.
     * @param hover path to hover image.
     * @param sound_config which sounds to play
     */
    Button(
        int x,
        int y,
        const std::string &image,
        const std::string &hover,
        ButtonSoundConfig sound_config = ButtonSoundConfig::NONE);

    /**
     * @brief Construct a new Button with given position, image, hover image and
     * click function. Will throw exception if images are not loaded properly.
     *
     * @param x pixel position.
     * @param y pixel position.
     * @param image path to image.
     * @param hover path to hover image.
     * @param func ptr to function to be called when clicked.
     * @param sound_config which sounds to play.
     * @param state_arg ptr to state to be passed to function.
     * @param volume volume of sounds.
     */
    Button(
        int x,
        int y,
        const std::string &image,
        const std::string &hover,
        void (*func)(Context &, GameState *),
        ButtonSoundConfig sound_config = ButtonSoundConfig::NONE,
        GameState *state_arg = nullptr,
        float volume = 100.0f);

    ~Button() = default;

    Button(const Button &) = delete;
    Button &operator=(const Button &) = delete;

    /**
     * @brief Render button. Will be called once every frame.
     * 
     * @param window window to draw on.
     */
    void render(sf::RenderWindow &window) const override;

    /**
     * @brief Update button. Will be called once every frame.
     * 
     * @param context[in,out] context class containing useful data.
     */
    void update(Context &context) override;

    /**
     * @brief Handle event. If click_function is set it will be called.
     *
     * @param event event to be handled.
     * @param context context[in,out] class containing useful data.
     * @return true if button is clicked.
     * @return false otherwise.
     */
    bool handle_click(const sf::Event &event, Context &context);

    /**
     * @brief Set the buttons click function. Click function is a function pointer
     * to a void function with Context& and GameState* as argument, ie
     * void func(Context &, GameState*).
     *
     * @param func function pointer.
     * @param state_arg ptr to game state to be passed to function.
     */
    void set_click_function(void (*func)(Context &, GameState *), GameState *state_arg = nullptr);

    /**
     * @brief Set sound to be played when button is clicked. Hover sound will not
     * be changed. Will throw exception if sound is not loaded properly.
     *
     * @param sound_path path to click sound.
     * @param sound_config option to play sound on hover, click, both or none.
     * Defaults to both.
     */
    void set_click_sound(
        const std::string &sound_path,
        ButtonSoundConfig sound_config = ButtonSoundConfig::HOVER_AND_CLICK);

    /**
     * @brief Set sound to be played when button is hovered. Click sound will not
     * be changed. Will throw exception if sound is not loaded properly.
     *
     * @param hover_sound_path path to hover sound.
     * @param sound_config option to play sound on hover, click, both or none.
     * Defaults to both.
     */
    void set_hover_sound(
        const std::string &hover_sound_path,
        ButtonSoundConfig sound_config = ButtonSoundConfig::HOVER_AND_CLICK);

    /**
     * @brief Set the sound of the button. Will throw exception if sound is not
     * loaded properly.
     *
     * @param sound_path path to click sound.
     * @param hover_sound_path path to hover sound.
     * @param sound_config option to play sound on hover, click both or none.
     * Defaults to both.
     */
    void set_sound(
        const std::string &sound_path,
        const std::string &hover_sound_path,
        ButtonSoundConfig sound_config = ButtonSoundConfig::HOVER_AND_CLICK);

    /**
     * @brief Set sound configuration. This will change which sounds are played.
     *
     * @param sound_config option to play sound on hover, click, both or none.
     * Defaults to both.
     */
    void set_config(ButtonSoundConfig sound_config);

    /**
     * @brief Set volume of sounds. Volume is a float between 0.0f and 100.0f.
     * 
     * @param volume volume of sounds.
     */
    void set_volume(float volume);

private:
    sf::Sprite m_button_sprite;
    const sf::Texture &m_image, &m_hover_image;
    const bool m_hoverable;
    bool m_hovering;

    // Function pointer and state to be passed to function.
    void (*m_click_function)(Context &, GameState *);
    GameState *m_game_state_arg;

    // Sound
    ButtonSoundConfig m_sound_config;
    sf::Sound &m_click_sound;
    sf::Sound &m_hover_sound;
};

/*===============================RectangleBar=================================*/

/**
 * @brief  A rectangular bar that can be used to display health, mana, etc. 
 * 
 * @tparam T type of max and current value. Must be a numeric type (float or int).
 */
template <typename T>
class RectangleBar : public DynamicUI
{
public:
    /**
     * @brief Construct a new Rectangle Bar object. Position and size are defaulted
     * to zero. Background color is defaulted to transparent and foreground color is white.
     * 
     * @param max_value reference to max value.
     * @param current_value reference to current value.
     */
    RectangleBar(const T &max_value, const T &current_value);

    /**
     * @brief Construct a new Rectangle Bar with given position, size, background
     * color and foreground/bar color.
     * 
     * @param x pixel position.
     * @param y pixel position.
     * @param width width of bar.
     * @param height height of bar.
     * @param max_value reference to max value.
     * @param current_value reference to current value.
     * @param background_color RGBA background color.
     * @param bar_color RGBA bar color.
     */
    RectangleBar(
        float x,
        float y,
        float width,
        float height,
        const T &max_value,
        const T &current_value,
        const sf::Color &background_color,
        const sf::Color &bar_color);

    /**
     * @brief Render bar based on current- and max value.
     * 
     * @param window window to draw on.
     */
    void render(sf::RenderWindow &window) const override;

    /**
     * @brief Update bar. Will update bar size based on current- and max value.
     * 
     * @param context[in,out] class containing useful data.
     */
    void update(Context &context) override;

    /**
     * @brief Update bar. Will update bar size based on current- and max value.
     * Is equivalent to calling update(Context &context).
     */
    void update();

    /**
     * @brief Set origin position of bar. Origin is the center of the bar. Based
     * on current width and height.
     * 
     * @note Should set size before setting position for correct origin.
     * 
     * @param x pixel position
     * @param y pixel position
     */
    void set_position(float x, float y);

    /**
     * @brief Set size of bar.
     * 
     * @param width width of bar.
     * @param height height of bar.
     */
    void set_size(float width, float height);

    /**
     * @brief Set background color of the bar.
     * 
     * @param background_color RGBA color.
     */
    void set_background_color(const sf::Color &background_color);

    /**
     * @brief Set bar color
     * 
     * @param bar_color RGBA color.
     */
    void set_bar_color(const sf::Color &bar_color);

    /**
     * @brief Set the outline of the bar.
     * 
     * @param outline_color color of outline.
     * @param outline_thickness thickness of outline in pixels.
     */
    void set_outline_color(
        const sf::Color &outline_color, float outline_thickness = 1.0f);

private:
    float m_width;
    const T &m_max_value;
    const T &m_current_value;
    float m_old_max_value;
    float m_old_value;
    sf::RectangleShape m_bar;
    sf::RectangleShape m_background;
};

typedef RectangleBar<int> IntRectangleBar;
typedef RectangleBar<float> FloatRectangleBar;