#include "normalmode.hpp"
#include "mainmenu.hpp"
#include "resourcemanager.hpp"
#include "enemyboss.hpp"
#include "enemyboss2.hpp"

#include "player.hpp"
#include "pausemenu.hpp"
#include "bossmode.hpp"
#include "endscreen.hpp"

#include <sstream>
#include <random>

BossMode::BossMode(GameMode *previous_state, Player *current_player,
                   unsigned int boss_level, unsigned int player_level)
    : GameMode{{current_player}, current_player},
      m_boss{nullptr},
      m_to_normal{false},
      m_to_normal_time{},
      m_time{5.f},
      m_previous_state{previous_state},
      m_scoreboard{},
      m_level_number_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},
      m_player_score_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},
      m_player_health_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},
      m_boss_countdown_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},

      m_boss_level{boss_level},
      m_player_level{player_level}
{
}

BossMode::~BossMode()
{
    GameMode::remove_player(false);
    delete m_previous_state;
}

void BossMode::render(sf::RenderWindow &window) const
{
    GameMode::render(window);
    window.draw(m_scoreboard);
    render_texts(window);
}
void BossMode::render_texts(sf::RenderWindow &window) const
{
    window.draw(m_level_number_text);
    window.draw(m_player_score_text);
    window.draw(m_player_health_text);
}

void BossMode::update(Context &context)
{
    update_texts();
    GameMode::update(context);

    Player *player = GameMode::get_player();
    if (player == nullptr)
    {
        return;
    }

    if (m_boss->is_removed() && !m_to_normal)
    {
        m_to_normal = true;
        m_to_normal_time = m_clock.getElapsedTime();
    }
    else if (m_to_normal)
    {
        to_normal(context);
    }
}

void BossMode::update_texts()
{
    {
        std::stringstream ss{};
        ss << m_player_level;
        m_level_number_text.setString(ss.str());

        float text_width{m_level_number_text.getLocalBounds().width};
        float text_height{m_level_number_text.getLocalBounds().height};
        m_level_number_text.setOrigin({text_width / 2.f, text_height});
    }

    // Player related information.
    Player *player = GameMode::get_player();
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

void BossMode::handle(const sf::Event &event, Context &context)
{
    GameMode::handle(event, context);
}

void BossMode::init(const GameConfiguration &gc)
{
    if (m_paused)
    {
        // Adjust the time to normal to account for the time paused.
        sf::Time time_paused = m_clock.getElapsedTime() - m_pause_time;
        m_to_normal_time += time_paused;
        GameMode::play_music();
        return;
    }
    init_texts();

    std::string base_path{"assets/images/normalmode/"};
    m_scoreboard.setTexture(ResourceManager::load_texture(base_path + "scoreboard.png"));
    m_scoreboard.setPosition(0, 12);

    const BossModeData &data{gc.get_boss_mode_data()};
    m_time = data.grace_period;

    sf::Vector2f pos{data.boss_pos_x, data.boss_pos_y};
    //get the right boss 
    if (m_boss_level >= 3)
    {
        m_boss_level = rand() % 2 + 1; 
    }


    if (m_boss_level == 1)
    {
        m_boss = new EnemyBoss{data.boss_data, pos.x, pos.y, 1};
    }
    else if (m_boss_level == 2)
    {
        m_boss = new EnemyBoss2{data.boss_data, pos.x, pos.y + 70, 1};
    }

    GameMode::spawn_object(m_boss);
    GameMode::set_background("assets/images/background.png", gc.get_window_size());
    GameMode::set_music("assets/sounds/boss_music.ogg", gc.get_data().music_volume);
    GameMode::play_music();
}

void BossMode::init_texts()
{
    m_level_number_text.setPosition(26.f, 39.f);
    m_level_number_text.setFillColor(sf::Color::White);

    m_player_score_text.setPosition(132.f, 39.f);
    m_player_score_text.setFillColor(sf::Color::White);

    m_player_health_text.setPosition(0.f, 100.f);
    m_player_health_text.setFillColor(sf::Color::White);
}

void BossMode::to_normal(Context &context)
{
    if ((m_clock.getElapsedTime() - m_to_normal_time).asSeconds() > m_time)
    {
        context.set_next_state(m_previous_state);
        m_previous_state = nullptr;
    }
}