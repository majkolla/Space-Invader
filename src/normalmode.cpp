#include "normalmode.hpp"
#include "pausemenu.hpp"
#include "mainmenu.hpp"
#include "resourcemanager.hpp"
#include "player.hpp"
#include "enemyminion.hpp"
#include "enemymover.hpp"
#include "enemymultishot.hpp"
#include "enemytank.hpp"
#include "bossmode.hpp"

#include <sstream>
#include <iomanip>
#include <random>
#include <cmath>

NormalMode::NormalMode()
    : GameMode(),
      m_spawn_time{2.5f},
      m_spawn_time_multiplier{0.95f},
      m_spawn_time_min{0.5f},
      m_boss_spawn_time{45.f},
      m_current_boss_time{0.f},
      m_prev_spawn_time{},
      m_prev_boss_spawn_time{},
      m_boss_counter{1},
      m_spawn_zone{0},
      m_spawn_zones{2},
      m_spawn_seed{0},
      m_level_rating{0},
      m_level_inc_rate{10.f},
      m_current_level_time{0.f},
      m_level_time{},
      m_level_up_sound{},
      m_scoreboard{},
      m_level_number_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},
      m_player_score_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},
      m_player_health_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},
      m_boss_countdown_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},
      m_boss_warning_rect{},
      m_level_bar{m_level_inc_rate, m_current_level_time},
      m_boss_countdown_bar{m_boss_spawn_time, m_current_boss_time},
      m_minion_data{}
{
}

NormalMode::~NormalMode()
{
}

void NormalMode::render(sf::RenderWindow &window) const
{
    GameMode::render(window);
    window.draw(m_scoreboard);
    render_bars(window);
    render_texts(window);
}

void NormalMode::update(Context &context)
{
    handle_time(context);
    update_texts();
    GameMode::update(context);
}

void NormalMode::handle(const sf::Event &event, Context &context)
{
    GameMode::handle(event, context);
}

void NormalMode::init(const GameConfiguration &gc)
{
    if (m_paused)
    {
        // Adjust times based on pause time.
        sf::Time time_paused = m_clock.getElapsedTime() - m_pause_time;
        m_prev_boss_spawn_time += time_paused;
        m_level_time += time_paused;
        GameMode::set_music_volume(gc.get_data().music_volume);
        GameMode::play_music();
        m_paused = false;
        return;
    }

    init_ui(gc);
    init_data(gc.get_normal_mode_data());

    int width{static_cast<int>(gc.get_window_size().x)};
    int height{static_cast<int>(gc.get_window_size().y)};

    m_level_up_sound.setBuffer(
        ResourceManager::load_sound_buffer("assets/sounds/normalmode/level.wav"));
    m_level_up_sound.setVolume(gc.get_data().effects_volume);

    Player *player = new Player{
        gc.get_normal_mode_data().player_data, width / 2.f, height - height / 4.f};
    GameMode::set_player(player);

    GameMode::set_background("assets/images/background.png", gc.get_window_size());
    GameMode::set_music("assets/sounds/normal_music.ogg", gc.get_data().music_volume);
    GameMode::play_music();
}

void NormalMode::render_texts(sf::RenderWindow &window) const
{
    window.draw(m_level_number_text);
    window.draw(m_player_score_text);
    window.draw(m_player_health_text);
}

void NormalMode::render_bars(sf::RenderWindow &window) const
{
    if (m_boss_spawn_time - m_current_boss_time <= std::ceil(m_boss_spawn_time * 0.1f))
    {
        window.draw(m_boss_warning_rect);
        window.draw(m_boss_countdown_text);
    }
    m_boss_countdown_bar.render(window);
    m_level_bar.render(window);
}

void NormalMode::update_texts()
{
    {
        std::stringstream ss{};
        ss << m_level_rating;
        m_level_number_text.setString(ss.str());
        // Make sure the text is centered.
        float text_width{m_level_number_text.getLocalBounds().width};
        float text_height{m_level_number_text.getLocalBounds().height};
        m_level_number_text.setOrigin({text_width / 2.f, text_height});
    }

    if (m_boss_spawn_time - m_current_boss_time <= std::round(m_boss_spawn_time * 0.1f))
    {
        std::stringstream ss{};
        ss << "Boss incoming " << std::fixed << std::setprecision(1) << std::setw(3)
           << m_boss_spawn_time - m_current_boss_time;
        m_boss_countdown_text.setString(ss.str());
        // Make sure the text is centered.
        float text_width{m_boss_countdown_text.getLocalBounds().width};
        float text_height{m_boss_countdown_text.getLocalBounds().height};
        m_boss_countdown_text.setOrigin({text_width / 2.f, text_height});
    }

    // Player related information.
    Player *player{GameMode::get_player()};
    if (player == nullptr)
        return;
    {
        std::stringstream ss{};
        ss << player->get_score();
        m_player_score_text.setString(ss.str());
        float text_width{m_player_score_text.getLocalBounds().width};
        float text_height{m_player_score_text.getLocalBounds().height};
        m_player_score_text.setOrigin({text_width / 2.f, text_height});
    }
    {
        std::stringstream ss{};
        ss << "Health - " << player->get_health();
        m_player_health_text.setString(ss.str());
    }
}

void NormalMode::handle_time(Context &context)
{
    m_current_boss_time = (m_clock.getElapsedTime() - m_prev_boss_spawn_time).asSeconds();
    m_boss_countdown_bar.update();

    m_current_level_time = (m_clock.getElapsedTime() - m_level_time).asSeconds();
    m_level_bar.update();

    if ((m_clock.getElapsedTime() - m_prev_spawn_time).asSeconds() >= m_spawn_time)
    {
        GameMode::spawn_object(get_enemy(context.get_window_size()));
        m_prev_spawn_time = m_clock.getElapsedTime();
    }
    if (m_current_boss_time >= m_boss_spawn_time)
    {
        m_prev_boss_spawn_time = m_clock.getElapsedTime();
        to_boss(context);
        return;
    }
    if (m_boss_spawn_time - m_current_boss_time <= std::ceil(m_boss_spawn_time * 0.1f))
    {
        GameMode::fade_out_music(std::ceil(m_boss_spawn_time * 0.1f));
    }
    if (m_current_level_time >= m_level_inc_rate)
    {
        (m_level_inc_rate < 60.f) ? m_level_inc_rate *= 1.25f : m_level_inc_rate = 60.f;
        m_level_rating++;
        m_spawn_time *= 0.95f;
        m_level_time = m_clock.getElapsedTime();
        m_level_up_sound.play();
    }
}

void NormalMode::init_texts(const GameConfiguration &gc)
{
    const BossModeData &boss_data{gc.get_boss_mode_data()};

    m_level_number_text.setPosition(26.f, 39.f);
    m_level_number_text.setFillColor(sf::Color::White);

    m_player_score_text.setPosition(132.f, 39.f);
    m_player_score_text.setFillColor(sf::Color::White);

    m_player_health_text.setPosition(0.f, 100.f);
    m_player_health_text.setFillColor(sf::Color::White);

    m_boss_countdown_text.setPosition(boss_data.boss_pos_x, boss_data.boss_pos_y);
    m_boss_countdown_text.setFillColor(sf::Color::Red);
}

void NormalMode::init_bars(const GameConfiguration &gc)
{
    int width{static_cast<int>(gc.get_window_size().x)};

    m_boss_countdown_bar.set_size(width, 12.f);
    m_boss_countdown_bar.set_position(width / 2.f, 6.f);
    m_boss_countdown_bar.set_bar_color(sf::Color{0x384b56be});
    m_boss_countdown_bar.set_background_color(sf::Color::Transparent);

    m_level_bar.set_size(52.f, 52.f);
    m_level_bar.set_position(26.f, 39.f);
    m_level_bar.set_bar_color(sf::Color{0x384b56be});
    m_level_bar.set_background_color(sf::Color::Transparent);
}

void NormalMode::init_ui(const GameConfiguration &gc)
{
    init_texts(gc);
    init_bars(gc);

    m_boss_warning_rect.setSize({gc.get_data().window_width - 4.f, 250.f});
    m_boss_warning_rect.setOrigin(m_boss_warning_rect.getSize() / 2.f);
    m_boss_warning_rect.setPosition(m_boss_countdown_text.getPosition());
    m_boss_warning_rect.setFillColor(sf::Color{0xff00000e});
    m_boss_warning_rect.setOutlineColor(sf::Color::Red);
    m_boss_warning_rect.setOutlineThickness(2.f);

    std::string base_path{"assets/images/normalmode/"};
    m_scoreboard.setTexture(ResourceManager::load_texture(base_path + "scoreboard.png"));
    m_scoreboard.setPosition(0, 12);
}

void NormalMode::init_data(const NormalModeData &data)
{
    m_level_rating = data.start_level;
    m_boss_spawn_time = data.boss_spawn_time;
    m_spawn_time_multiplier = data.spawn_time_multiplier;
    m_spawn_time_min = data.spawn_time_min;

    m_spawn_time = std::max<float>(
        data.base_spawn_time * std::pow(m_spawn_time_multiplier, m_level_rating),
        m_spawn_time_min);

    m_spawn_seed = data.spawn_seed;

    m_level_inc_rate = data.level_increase_time;

    m_prev_boss_spawn_time = m_clock.getElapsedTime();
    m_prev_spawn_time = m_clock.getElapsedTime();

    // Save the minion data for later use.
    m_minion_data = data.minion_data;
}

GameObject *NormalMode::get_enemy(const sf::Vector2u &window_size)
{
    sf::Vector2f pos{get_enemy_position(window_size)};
    // Spawn a random enemy based on the level rating.
    switch (m_level_rating / 3)
    {
    case 0:
        return get_enemy_first(pos);

    case 1:
        return get_enemy_second(pos);

    case 2:
        return get_enemy_third(pos);

    case 3:
        return get_enemy_fourth(pos);

    default:
        return get_enemy_default(pos);
    }
}

sf::Vector2f NormalMode::get_enemy_position(const sf::Vector2u &window_size)
{
    // Update the spawn seed based on previous seed.
    std::mt19937 g{m_spawn_seed};
    std::uniform_int_distribution<unsigned int> seed_distr(
        0, std::numeric_limits<unsigned int>::max());
    m_spawn_seed = seed_distr(g);

    // Get a random position for the enemy.
    std::uniform_real_distribution<float> distr(0.05f, 0.95f);
    float interval = window_size.x / (m_spawn_zones + 1);
    float x = distr(g) * interval + m_spawn_zone * interval;
    float y{-distr(g) * 50.f + 20.f};
    (m_spawn_zone == m_spawn_zones) ? m_spawn_zone = 0 : m_spawn_zone++;
    return {x, y};
}

GameObject *NormalMode::get_enemy_first(const sf::Vector2f &pos) const
{
    return new EnemyMinion{m_minion_data, pos.x, pos.y, 1};
}

GameObject *NormalMode::get_enemy_second(const sf::Vector2f &pos) const
{
    int random = rand() % 10;
    if (random >= 0 && random <= 8)
    {
        int difficulty = rand() % 4;
        difficulty != 0 ? difficulty = 1 : difficulty = 2;
        return new EnemyMinion{m_minion_data, pos.x, pos.y, difficulty};
    }
    else
        return new EnemyMover{m_minion_data, pos.x, pos.y, 1};
}

GameObject *NormalMode::get_enemy_third(const sf::Vector2f &pos) const
{
    int random = rand() % 10;
    if (random >= 0 && random <= 6)
    {
        int difficulty = rand() % 4;
        difficulty != 0 ? difficulty = 1 : difficulty = 2;
        return new EnemyMinion{m_minion_data, pos.x, pos.y, difficulty};
    }
    else if (random >= 7 && random <= 8)
        return new EnemyMover{m_minion_data, pos.x, pos.y, 1};
    else
        return new EnemyTank{m_minion_data, pos.x, pos.y, 1};
}

GameObject *NormalMode::get_enemy_fourth(const sf::Vector2f &pos) const
{
    int random = rand() % 10;
    int difficulty{};
    if (random >= 0 && random <= 6)
    {
        difficulty = rand() % 4;
        difficulty != 0 ? difficulty = 2 : difficulty = 3;
        return new EnemyMinion{m_minion_data, pos.x, pos.y, difficulty};
    }
    else if (random >= 7 && random <= 8)
    {
        difficulty = rand() % 4;
        difficulty != 0 ? difficulty = 1 : difficulty = 2;
        return new EnemyMover{m_minion_data, pos.x, pos.y, difficulty};
    }
    else
        return new EnemyTank{m_minion_data, pos.x, pos.y, 1};
}

GameObject *NormalMode::get_enemy_default(const sf::Vector2f &pos) const
{
    int random = rand() % 20;
    int difficulty{};
    if (random >= 0 && random <= 10)
    {
        difficulty = rand() % 4;
        difficulty <= 2 ? difficulty = 2 : difficulty = 3;
        return new EnemyMinion{m_minion_data, pos.x, pos.y, difficulty};
    }
    else if (random >= 11 && random <= 15)
    {
        difficulty = rand() % 6;
        difficulty != 0 ? difficulty = 2 : difficulty = 3;
        return new EnemyMover{m_minion_data, pos.x, pos.y, difficulty};
    }
    else if (random >= 16 && random <= 18)
    {
        difficulty = rand() % 4;
        difficulty != 0 ? difficulty = 1 : difficulty = 2;
        return new EnemyTank{m_minion_data, pos.x, pos.y, difficulty};
    }
    else
        return new EnemyMultiShot{m_minion_data, pos.x, pos.y, 1};
}

void NormalMode::to_boss(Context &context)
{
    GameMode::clear_objects();
    GameMode::pause();
    m_prev_boss_spawn_time = m_clock.getElapsedTime();
    context.set_next_state(
        new BossMode{this,
                     GameMode::get_player(),
                     m_boss_counter, m_level_rating});
    m_boss_counter++;
}