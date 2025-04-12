#include "ui.hpp"
#include "resourcemanager.hpp"
#include <cmath>

Button::Button(int x, int y, const std::string &image, ButtonSoundConfig sound_config)
    : Button(x, y, image, nullptr, sound_config, nullptr)
{
}

Button::Button(
    int x,
    int y,
    const std::string &image,
    void (*func)(Context &, GameState *),
    ButtonSoundConfig sound_config,
    GameState *state_arg)
    : m_button_sprite{},
      m_image{ResourceManager::load_texture(image)},
      m_hover_image{m_image},
      m_hoverable{false},
      m_hovering{false},
      m_click_function{func},
      m_game_state_arg{state_arg},
      m_sound_config{sound_config},
      m_click_sound{ResourceManager::load_sound("assets/sounds/button.wav")},
      m_hover_sound{ResourceManager::load_sound("assets/sounds/button_h.wav")}

{
    m_button_sprite.setTexture(m_image);
    m_button_sprite.setOrigin(m_image.getSize().x / 2.f, m_image.getSize().y / 2.f);
    m_button_sprite.setPosition(x, y);
}

Button::Button(
    int x,
    int y,
    const std::string &image,
    const std::string &hover,
    ButtonSoundConfig sound_config)
    : Button(x, y, image, hover, nullptr, sound_config, nullptr)
{
}

Button::Button(
    int x,
    int y,
    const std::string &image,
    const std::string &hover,
    void (*func)(Context &, GameState *),
    ButtonSoundConfig sound_config,
    GameState *state_arg,
    float volume)
    : m_button_sprite{},
      m_image{ResourceManager::load_texture(image)},
      m_hover_image{ResourceManager::load_texture(hover)},
      m_hoverable{true},
      m_hovering{false},
      m_click_function{func},
      m_game_state_arg{state_arg},
      m_sound_config{sound_config},
      m_click_sound{ResourceManager::load_sound("assets/sounds/button.wav")},
      m_hover_sound{ResourceManager::load_sound("assets/sounds/button_h.wav")}
{
    m_button_sprite.setTexture(m_image);
    m_button_sprite.setOrigin(m_image.getSize().x / 2.f, m_image.getSize().y / 2.f);
    m_button_sprite.setPosition(x, y);
    m_click_sound.setVolume(volume);
    m_hover_sound.setVolume(volume);
}

void Button::render(sf::RenderWindow &window) const
{
    window.draw(m_button_sprite);
}

void Button::update(Context &context)
{
    if (!m_hoverable)
        return;

    sf::Vector2f mouse_pos{sf::Mouse::getPosition(context.get_window())};

    if (m_button_sprite.getGlobalBounds().contains(mouse_pos))
    {
        if ((m_hovering))
            return;

        if (m_sound_config == ButtonSoundConfig::HOVER ||
            m_sound_config == ButtonSoundConfig::HOVER_AND_CLICK)
        {
            m_hover_sound.play();
        }
        m_button_sprite.setTexture(m_hover_image, true);
        m_hovering = true;
    }
    else if (m_hovering)
    {
        m_button_sprite.setTexture(m_image, true);
        m_hovering = false;
    }
}

bool Button::handle_click(const sf::Event &event, Context &context)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        auto mb = event.mouseButton;
        if (!m_button_sprite.getGlobalBounds().contains(mb.x, mb.y))
        {
            return false;
        }

        if (m_sound_config == ButtonSoundConfig::CLICK ||
            m_sound_config == ButtonSoundConfig::HOVER_AND_CLICK)
        {
            m_click_sound.play();
        }

        if (m_click_function != nullptr)
            m_click_function(context, m_game_state_arg);
        return true;
    }
    return false;
}

void Button::set_click_function(
    void (*func)(Context &, GameState *), GameState *state_arg)
{
    m_click_function = func;
    m_game_state_arg = state_arg;
}

void Button::set_click_sound(
    const std::string &sound_path, ButtonSoundConfig sound_config)
{
    m_click_sound = ResourceManager::load_sound(sound_path);
    m_sound_config = sound_config;
}

void Button::set_hover_sound(
    const std::string &sound_path, ButtonSoundConfig sound_config)
{
    m_hover_sound = ResourceManager::load_sound(sound_path);
    m_sound_config = sound_config;
}

void Button::set_sound(
    const std::string &sound_path,
    const std::string &hover_sound_path,
    ButtonSoundConfig sound_config)
{
    m_click_sound = ResourceManager::load_sound(sound_path);
    m_hover_sound = ResourceManager::load_sound(hover_sound_path);
    m_sound_config = sound_config;
}

void Button::set_config(ButtonSoundConfig sound_config)
{
    m_sound_config = sound_config;
}

void Button::set_volume(float volume)
{
    m_click_sound.setVolume(volume);
    m_hover_sound.setVolume(volume);
}

/*==================================RectangleBar==============================*/

template <typename T>
RectangleBar<T>::RectangleBar(const T &max_value, const T &current_value)
    : RectangleBar{
          0.f, 0.f,
          0.f, 0.f,
          max_value, current_value,
          sf::Color::White, sf::Color::Transparent}
{
}

template <typename T>
RectangleBar<T>::RectangleBar(
    float x,
    float y,
    float width,
    float height,
    const T &max_value,
    const T &current_value,
    const sf::Color &color,
    const sf::Color &background_color)
    : m_width{width},
      m_max_value{max_value},
      m_current_value{current_value},
      m_old_max_value{},
      m_old_value{},
      m_bar{sf::Vector2f{width, height}},
      m_background{sf::Vector2f{width, height}}
{
    m_bar.setOrigin(width / 2.f, height / 2.f);
    m_bar.setPosition(x, y);
    m_bar.setFillColor(color);

    m_background.setOrigin(width / 2.f, height / 2.f);
    m_background.setPosition(x, y);
    m_background.setFillColor(background_color);
}

template <typename T>
void RectangleBar<T>::render(sf::RenderWindow &window) const
{
    window.draw(m_background);
    window.draw(m_bar);
}

template <typename T>
void RectangleBar<T>::update(Context &)
{
    update();
}

template <typename T>
void RectangleBar<T>::update()
{
    if (m_current_value != m_old_value || m_max_value != m_old_max_value)
    {
        m_bar.setSize(sf::Vector2f{
            m_width * (m_current_value / static_cast<float>(m_max_value)),
            m_bar.getSize().y});
        m_old_value = m_current_value;
        m_old_max_value = m_max_value;
    }
}

template <typename T>
void RectangleBar<T>::set_position(float x, float y)
{
    sf::Vector2f size = m_background.getSize();
    float y_origin{size.y / 2.f};
    float x_origin{size.x / 2.f};
    m_bar.setOrigin(x_origin, y_origin);
    m_bar.setPosition(x, y);
    m_background.setOrigin(x_origin, y_origin);
    m_background.setPosition(x, y);
}

template <typename T>
void RectangleBar<T>::set_size(float width, float height)
{
    m_bar.setSize(sf::Vector2f{width, height});
    m_background.setSize(sf::Vector2f{width, height});
    m_width = width;
}

template <typename T>
void RectangleBar<T>::set_bar_color(const sf::Color &color)
{
    m_bar.setFillColor(color);
}

template <typename T>
void RectangleBar<T>::set_background_color(const sf::Color &color)
{
    m_background.setFillColor(color);
}

template <typename T>
void RectangleBar<T>::set_outline_color(const sf::Color &color, float outline_thickness)
{
    m_background.setOutlineColor(color);
    m_background.setOutlineThickness(outline_thickness);
}

template class RectangleBar<int>;
template class RectangleBar<float>;