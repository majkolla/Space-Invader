#include "pausemenu.hpp"
#include "mainmenu.hpp"
#include "normalmode.hpp"
#include "ui.hpp"
#include "resourcemanager.hpp"

PauseMenu::PauseMenu(GameState *previous_state)
    : Menu(), m_previous_state{previous_state}
{
}

PauseMenu::~PauseMenu()
{
    delete m_previous_state;
}

void PauseMenu::render(sf::RenderWindow &window) const
{
    if (m_previous_state != nullptr)
    {
        m_previous_state->render(window);
    }

    sf::RectangleShape rect{
        {static_cast<float>(window.getSize().x),
         static_cast<float>(window.getSize().y)}};

    rect.setFillColor({0, 0, 0, 180});
    rect.setPosition(0, 0);

    window.draw(rect);
    Menu::render(window);
}

void PauseMenu::update(Context &context)
{
    Menu::update(context);
}

void PauseMenu::handle(const sf::Event &event, Context &context)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::P)
        {
            unpause(context);
            return;
        }
    }
    Menu::handle(event, context);
}

void PauseMenu::init(const GameConfiguration &gc)
{
    int width{static_cast<int>(gc.get_window_size().x)};
    int height{static_cast<int>(gc.get_window_size().y)};
    std::string base_path{"assets/images/menu/"};

    float volume = gc.get_data().effects_volume;

    // Add buttons;
    Menu::add_button(new Button{
        width / 2, height / 2, base_path + "resume.png", base_path + "resume_h.png",
        [](Context &c, GameState *state)
        {
            PauseMenu *pause_state = dynamic_cast<PauseMenu *>(state);
            pause_state->unpause(c);
        },
        ButtonSoundConfig::HOVER_AND_CLICK, this, volume});

    Menu::add_button(new Button{
        width / 2, height / 2 + 75, base_path + "restart.png", base_path + "restart_h.png",
        [](Context &c, GameState *)
        { c.set_next_state(new NormalMode{}); },
        ButtonSoundConfig::HOVER_AND_CLICK, nullptr, volume});

    Menu::add_button(new Button{
        width / 2, height / 2 + 150, base_path + "to_menu.png", base_path + "to_menu_h.png",
        [](Context &c, GameState *)
        { c.set_next_state(new MainMenu{}); },
        ButtonSoundConfig::HOVER_AND_CLICK, nullptr, volume});

    {
        sf::Texture &texture = ResourceManager::load_texture(base_path + "pause.png");
        sf::Sprite *sprite = new sf::Sprite{texture};
        sprite->setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
        sprite->setPosition(width / 2, height / 2);
        Menu::add_sprite(sprite);
    }
}

void PauseMenu::unpause(Context &context)
{
    context.set_next_state(m_previous_state);
    m_previous_state = nullptr;
}