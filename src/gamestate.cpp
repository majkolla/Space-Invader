#include "gamestate.hpp"
#include "gameobject.hpp"
#include "player.hpp"
#include "resourcemanager.hpp"
#include "endscreen.hpp"
#include "pausemenu.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <sstream>

/*===================================GameMode=================================*/

GameMode::GameMode()
    : GameMode({}, nullptr)
{
}

GameMode::GameMode(const std::vector<GameObject *> &objects)
    : GameMode{objects, nullptr}
{
}

GameMode::GameMode(const std::vector<GameObject *> &objects, Player *player)
    : m_clock{},
      m_pause_time{},
      m_paused{false},
      m_objects{objects},
      m_player{player},
      m_background{},
      m_music{},
      m_music_volume{0.f},
      m_music_fade_start{},
      m_music_fade_duration{0.f},
      m_music_fade_type{FadeType::None}
{
}

GameMode::~GameMode()
{
    for (GameObject *object : m_objects)
    {
        delete object;
    }
}

void GameMode::render(sf::RenderWindow &window) const
{
    window.draw(m_background);
    for (const GameObject *object : m_objects)
    {
        object->render(window);
    }
}

void GameMode::update(Context &context)
{
    // Update context with all currently spawned objects & set player.
    context.set_player(m_player);

    update_objects(context);
    collision_check();

    // If player is removed (dead), end game & skip deletion of objects. Otherwise,
    // the player will be deleted and the game will crash in next state.
    if (m_player != nullptr && m_player->is_removed())
    {
        context.set_next_state(new EndScreen{this, m_player});
        // Objects need to be spawned, because context throws an exception if
        // there objects remaining when it is destroyed.
        spawn_new_objects(context);
        return;
    }
    delete_removed_objects();
    spawn_new_objects(context);
    update_music();
}

void GameMode::handle(const sf::Event &event, Context &context)
{
    // All GamMode can be paused. This is done by pressing P.
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::P)
        {
            GameMode::pause();
            context.set_next_state(new PauseMenu{this});
            return;
        }
    }
    // Handle all objects.
    for (GameObject *object : m_objects)
    {
        if (object->handle(event, context))
            break;
    }
}

void GameMode::pause()
{
    pause_music();
    m_pause_time = m_clock.getElapsedTime();
    m_paused = true;
}

void GameMode::update_objects(Context &context)
{
    for (GameObject *object : m_objects)
    {
        object->update(context);
    }
}

void GameMode::collision_check()
{
    // Inspired by lecture by Christoffer Holm. (https://www.ida.liu.se/~TDDC76/current/fo/index.sv.shtml)
    for (unsigned int i{0}; i < m_objects.size(); i++)
    {
        for (unsigned int j{i + 1}; j < m_objects.size(); j++)
        {
            if (m_objects.at(i)->collides(m_objects.at(j)))
            {
                m_objects.at(i)->collision(m_objects.at(j));
                m_objects.at(j)->collision(m_objects.at(i));
            }
        }
    }
}

void GameMode::delete_removed_objects()
{
    // Inspired by lecture by Christoffer Holm. (https://www.ida.liu.se/~TDDC76/current/fo/index.sv.shtml)
    for (unsigned int i{0}; i < m_objects.size();)
    {
        if (m_objects.at(i)->is_removed())
        {
            std::swap(m_objects.at(i), m_objects.back());
            delete m_objects.back();
            m_objects.pop_back();
        }
        else
        {
            i++;
        }
    }
}

void GameMode::spawn_new_objects(Context &context)
{
    std::vector<GameObject *> new_objects{};
    context.get_new_objects(new_objects);

    // Spawn new objects.
    for (GameObject *object : new_objects)
    {
        m_objects.push_back(object);
    }
}

void GameMode::set_player(Player *player)
{
    remove_player();
    spawn_object(player);
    m_player = player;
}

void GameMode::remove_player(bool delete_player)
{
    if (m_player != nullptr)
    {
        auto it = std::find(m_objects.begin(), m_objects.end(), m_player);
        if (it != m_objects.end())
        {
            // Remove old player from objects vector.
            std::swap(*it, m_objects.back());
            m_objects.pop_back();
            if (delete_player)
                delete m_player;
            m_player = nullptr;
        }
    }
}

Player *GameMode::get_player() const
{
    return m_player;
}

void GameMode::spawn_object(GameObject *object)
{
    if (object == nullptr)
        throw std::logic_error("SpawnERROR: tried to spawn nullptr.");
    m_objects.push_back(object);
}

void GameMode::set_background(const std::string &path, const sf::Vector2u &window_size)
{
    // Scale background to window size based on original image aspect ratio. (Preserve aspect ratio)
    sf::Texture &texture = ResourceManager::load_texture(path);
    m_background.setTexture(texture);
    float aspect_ratio = static_cast<float>(texture.getSize().x) / texture.getSize().y;
    float x_scale = static_cast<float>(window_size.x) / texture.getSize().x;
    float y_scale = static_cast<float>(window_size.y) / texture.getSize().y;
    float scale = std::max(x_scale, y_scale * aspect_ratio);
    m_background.setPosition(0, 0);
    m_background.setScale(scale, scale);
}

void GameMode::clear_objects(bool delete_player)
{
    for (GameObject *object : m_objects)
    {
        if (object != m_player || delete_player)
            delete object;
    }
    m_objects.clear();
    if (!delete_player)
    {
        m_objects.push_back(m_player);
    }
    else
    {
        m_player = nullptr;
    }
}

void GameMode::set_music(const std::string &path, float volume, bool loop)
{
    if (!m_music.openFromFile(path))
    {
        std::stringstream ss{};
        ss << "GameModeERROR: could not load music file: " << path;
        throw std::logic_error(ss.str());
    }

    m_music.setVolume(volume);
    m_music.setLoop(loop);
}

void GameMode::set_music_volume(float volume)
{
    m_music.setVolume(volume);
    m_music_fade_type = FadeType::None;
}

void GameMode::play_music()
{
    m_music.play();
    m_music_fade_type = FadeType::None;
}

void GameMode::pause_music()
{
    m_music.pause();
    m_music_fade_type = FadeType::None;
}

void GameMode::loop_music(bool loop)
{
    m_music.setLoop(loop);
}

void GameMode::fade_out_music(float duration)
{
    if (m_music_fade_type != FadeType::None)
        return;
    m_music_fade_type = FadeType::FadeOut;
    m_music_fade_start = m_clock.getElapsedTime();
    m_music_fade_duration = duration;
    m_music_volume = m_music.getVolume();
}

void GameMode::fade_in_music(float duration, float target_volume)
{
    if (m_music_fade_type != FadeType::None)
        return;

    if (m_music.getStatus() != sf::Music::Status::Playing)
        m_music.play();

    m_music.setVolume(0.f);
    m_music_fade_type = FadeType::FadeIn;
    m_music_fade_start = m_clock.getElapsedTime();
    m_music_fade_duration = duration;
    m_music_volume = target_volume;
}

const std::vector<GameObject *> &GameMode::get_objects() const
{
    return m_objects;
}

sf::Music &GameMode::get_music()
{
    return m_music;
}

void GameMode::update_music()
{
    if (m_music_fade_type == FadeType::None)
        return;

    if (m_music_fade_type == FadeType::FadeOut)
    {
        sf::Time elapsed = m_clock.getElapsedTime() - m_music_fade_start;
        float progress = elapsed.asSeconds() / m_music_fade_duration;
        if (progress >= 1)
        {
            m_music_fade_duration = 0;
            m_music_fade_type = FadeType::None;
            m_music.setVolume(0);
            m_music.pause();
        }
        else
        {
            m_music.setVolume(m_music_volume * (1 - progress));
        }
    }
    else if (m_music_fade_type == FadeType::FadeIn)
    {
        sf::Time elapsed = m_clock.getElapsedTime() - m_music_fade_start;
        float progress = elapsed.asSeconds() / m_music_fade_duration;
        if (progress >= 1)
        {
            m_music_fade_duration = 0;
            m_music_fade_type = FadeType::None;
            m_music.setVolume(m_music_volume);
        }
        else
        {
            m_music.setVolume(m_music_volume * progress);
        }
    }
}

/*======================================Menu==================================*/

Menu::Menu()
    : m_buttons{}, m_sprites{}
{
}

Menu::~Menu()
{
    for (Button *button : m_buttons)
    {
        delete button;
    }

    for (sf::Sprite *sprite : m_sprites)
    {
        delete sprite;
    }
}

void Menu::render(sf::RenderWindow &window) const
{
    for (const sf::Sprite *sprite : m_sprites)
    {
        window.draw(*sprite);
    }

    for (const Button *button : m_buttons)
    {
        button->render(window);
    }
}

void Menu::update(Context &context)
{
    for (Button *button : m_buttons)
    {
        button->update(context);
    }
}

void Menu::handle(const sf::Event &event, Context &context)
{
    for (Button *button : m_buttons)
    {
        if (button->handle_click(event, context))
            break;
    }
}

void Menu::add_button(Button *button)
{
    if (button == nullptr)
        throw std::logic_error("MenuERROR: tried to add nullptr button");
    m_buttons.push_back(button);
}

void Menu::add_sprite(sf::Sprite *sprite)
{
    if (sprite == nullptr)
        throw std::logic_error("MenuERROR: tried to add nullptr sprite");
    m_sprites.push_back(sprite);
}
