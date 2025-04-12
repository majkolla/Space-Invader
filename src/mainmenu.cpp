#include "mainmenu.hpp"
#include "normalmode.hpp"
#include "resourcemanager.hpp"
#include "enemy.hpp"
#include "enemyminion.hpp"

MainMenu::MainMenu()
    : Menu(), m_show_help(false), m_help_sprite{}
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::render(sf::RenderWindow &window) const
{
    Menu::render(window);
    if (m_show_help)
    {
        window.draw(m_help_sprite);
    }
}

void MainMenu::update(Context &context)
{
    Menu::update(context);
}

void MainMenu::handle(const sf::Event &event, Context &context)
{
    Menu::handle(event, context);
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            m_show_help = false;
        }
    }
}

void MainMenu::init(const GameConfiguration &gc)
{
    int width{static_cast<int>(gc.get_window_size().x)};
    int height{static_cast<int>(gc.get_window_size().y)};
    std::string base_path{"assets/images/menu/"};

    float volume = gc.get_data().effects_volume;

    // Add buttons;
    Menu::add_button(new Button{
        width / 2 - 110, height - 100, base_path + "play.png", base_path + "play_h.png",
        [](Context &c, GameState *)
        { c.set_next_state(new NormalMode{}); },
        ButtonSoundConfig::HOVER_AND_CLICK, nullptr, volume});

    Menu::add_button(new Button{
        width / 2 + 110, height - 100, base_path + "exit.png", base_path + "exit_h.png",
        [](Context &c, GameState *)
        { c.exit(); },
        ButtonSoundConfig::HOVER_AND_CLICK, nullptr, volume});

    Menu::add_button(new Button{
        width - 40, 40, base_path + "help.png", base_path + "help_h.png",
        [](Context &, GameState *self)
        {
            MainMenu *menu = dynamic_cast<MainMenu *>(self);
            menu->toggle_help();
        },
        ButtonSoundConfig::HOVER_AND_CLICK, this, volume});

    // Add background
    {
        sf::Texture &texture = ResourceManager::load_texture("assets/images/background.png");
        sf::Sprite *sprite = new sf::Sprite{texture};
        float x_scale = static_cast<float>(width) / texture.getSize().x;
        float y_scale = static_cast<float>(height) / texture.getSize().y;
        sprite->setPosition(0, 0);
        sprite->setScale(x_scale, y_scale);
        Menu::add_sprite(sprite);
    }

    // Add game name.
    {
        sf::Texture &texture = ResourceManager::load_texture(base_path + "name.png");
        sf::Sprite *sprite = new sf::Sprite{texture};
        sprite->setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
        sprite->setPosition(width / 2, 200);
        Menu::add_sprite(sprite);
    }

    // Add help menu sprite.
    {
        sf::Texture &texture = ResourceManager::load_texture(base_path + "help_menu.png");
        m_help_sprite.setTexture(texture);
        m_help_sprite.setOrigin(texture.getSize().x, 0);
        m_help_sprite.setPosition(width - 55, 55);
    }
}

void MainMenu::toggle_help()
{
    m_show_help = !m_show_help;
}