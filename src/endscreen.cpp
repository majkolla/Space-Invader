// kills, bosskills, score
#include "normalmode.hpp"
#include "mainmenu.hpp"
#include "resourcemanager.hpp"
#include "enemyboss.hpp"
#include "player.hpp"
#include "endscreen.hpp"

#include <sstream>
#include <iomanip>

EndScreen::EndScreen(GameState *previous_state, Player *current_player)
    : Menu(),
      m_previous_state{previous_state},
      m_current_player{current_player},
      m_scoreboard{},
      m_player_score_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},
      m_death_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},
      m_highscore_text{"", ResourceManager::load_font("assets/font/Aquire.otf")},
      m_current_highscore{},
      m_new_highscore{}
{
}

EndScreen::~EndScreen()
{
    delete m_previous_state;
}

void EndScreen::render(sf::RenderWindow &window) const
{
    if (m_previous_state != nullptr)
    {
        m_previous_state->render(window);
    }

    sf::RectangleShape rect{{static_cast<float>(window.getSize().x),
                             static_cast<float>(window.getSize().y)}};
    rect.setFillColor({40, 0, 0, 180});
    rect.setPosition(0, 0);

    window.draw(rect);

    render_texts(window);
    Menu::render(window);
}

void EndScreen::update(Context &context)
{
    Menu::update(context);
    update_texts();
}

void EndScreen::handle(const sf::Event &event, Context &context)
{
    Menu::handle(event, context);
}

void EndScreen::init(const GameConfiguration &gc)
{
    int width{static_cast<int>(gc.get_window_size().x)};
    int height{static_cast<int>(gc.get_window_size().y)};

    std::string path{"assets/images/menu/"};

    check_highscore(gc);

    init_texts(gc);

    float volume{gc.get_data().effects_volume};
    Menu::add_button(new Button{
        width / 2, height / 2, path + "restart.png", path + "restart_h.png",
        [](Context &c, GameState *)
        { c.set_next_state(new NormalMode{}); },
        ButtonSoundConfig::HOVER_AND_CLICK, nullptr, volume});

    Menu::add_button(new Button{
        width / 2, height / 2 + 150, path + "to_menu.png", path + "to_menu_h.png",
        [](Context &c, GameState *)
        { c.set_next_state(new MainMenu{}); },
        ButtonSoundConfig::HOVER_AND_CLICK, nullptr, volume});
    {
        std::string base_path{"assets/images/normalmode/"};
        m_scoreboard.setTexture(ResourceManager::load_texture(base_path + "scoreboard.png"));
        m_scoreboard.setPosition(width / 2, height / 2 + 300);
    }
    {
        if (m_new_highscore)
        {
            set_new_highscore(gc);
        }
        else
        {
            m_current_highscore = gc.get_high_score();
        }
    }
}

void EndScreen::init_texts(const GameConfiguration &gc)
{
    int width{static_cast<int>(gc.get_window_size().x)};
    int height{static_cast<int>(gc.get_window_size().y)};

    // game score text
    m_player_score_text.setPosition(width / 2, height / 2 - 150);
    m_player_score_text.setFillColor(sf::Color::White);

    // Death text
    m_death_text.setPosition(width / 2, height / 2 - 400);
    m_death_text.setFillColor(sf::Color::Red);

    // Highscore text
    m_highscore_text.setPosition(width / 2, height / 2 - 250);
    m_highscore_text.setFillColor(sf::Color::White);
}

void EndScreen::update_texts()
{
    {
        std::stringstream ss{};
        ss << "Score " << m_current_player->get_score() << "    minion kills " << m_current_player->get_kills()
           << "    boss kills " << m_current_player->get_boss_kills();
        m_player_score_text.setString(ss.str());
        float text_width = m_player_score_text.getLocalBounds().width;
        float text_height = m_player_score_text.getLocalBounds().height;
        m_player_score_text.setOrigin({text_width / 2.f, text_height});
    }
    {
        std::stringstream ss{};
        ss << "you died";
        m_death_text.setString(ss.str());
        float text_width{m_death_text.getLocalBounds().width};
        float text_height{m_death_text.getLocalBounds().height};
        m_death_text.setOrigin({text_width / 2.f, text_height});
    }
    if (m_new_highscore)
    {
        std::stringstream ss{};
        ss << "congratulations space warrior\n you broke the highscore "
           << m_current_player->get_score();
        m_highscore_text.setString(ss.str());
        float text_width{m_highscore_text.getLocalBounds().width};
        float text_height{m_highscore_text.getLocalBounds().height};
        m_highscore_text.setOrigin({text_width / 2.f, text_height});
    }
    else
    {
        std::stringstream ss{};
        ss << "highscore " << m_current_highscore;
        m_highscore_text.setString(ss.str());
        float text_width{m_highscore_text.getLocalBounds().width};
        float text_height{m_highscore_text.getLocalBounds().height};
        m_highscore_text.setOrigin({text_width / 2.f, text_height});
    }
}

void EndScreen::render_texts(sf::RenderWindow &window) const
{
    window.draw(m_player_score_text);
    window.draw(m_death_text);
    window.draw(m_highscore_text);
}

void EndScreen::check_highscore(const GameConfiguration &gc)
{
    unsigned int current_highscore{gc.get_high_score()};

    if (m_current_player->get_score() > (int)current_highscore)
    {
        m_new_highscore = true;
        return;
    }
    m_new_highscore = false;
}

void EndScreen::set_new_highscore(const GameConfiguration &gc)
{
    gc.update_high_score(m_current_player->get_score());
}