#include "game.hpp"
#include "pausemenu.hpp"
#include "mainmenu.hpp"
#include "normalmode.hpp"
#include "bossmode.hpp"
#include "endscreen.hpp"

Game::Game(const GameConfiguration& gc, GameState* start)
    : m_window{sf::VideoMode{gc.get_window_size().x, gc.get_window_size().y}, gc.get_data().title}, 
    m_current_state{start}, 
    m_configuration{gc}
{
    sf::Image icon{};
     if (!icon.loadFromFile("assets/images/icon.png"))
         throw std::logic_error("Cannot open file!");

    m_window.setIcon(28, 28, icon.getPixelsPtr());
    m_window.setFramerateLimit(gc.get_data().fps);
    m_current_state->init(m_configuration);
}

Game::~Game()
{
    delete m_current_state;
    m_window.close();
}

void Game::run()
{
    if (m_current_state == nullptr)
    {
        throw std::logic_error("GameERROR: Current state is nullptr.");
    }

    sf::Clock clock{};

    while (m_window.isOpen())
    {
        Context context{clock.restart(), m_window};
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            handle(event);
            m_current_state->handle(event, context);
        }

        m_current_state->update(context);

        m_window.clear();
        m_current_state->render(m_window);
        m_window.display();

        handle_context(context);
    }
}

void Game::set_state(GameState *new_state)
{
    if (m_current_state != nullptr)
        delete m_current_state;

    m_current_state = new_state;
    m_current_state->init(m_configuration);
}

void Game::handle(const sf::Event &event)
{
    if (event.type == sf::Event::Closed)
    {
        m_window.close();
    }
    else if (event.type == sf::Event::Resized)
    {
        sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
        m_window.setView(sf::View(visible_area));
        m_configuration.update_window_size({event.size.width, event.size.height});
    }
    else if (event.type == sf::Event::LostFocus)
    {
        // Should pause the game if you loose focus, but only if you are in GameMode state.
        if (GameMode *state = dynamic_cast<GameMode *>(m_current_state))
        {
            state->pause();
            m_current_state = new PauseMenu{m_current_state};
            m_current_state->init(m_configuration);
        }
    }
}

void Game::handle_context(Context &context)
{
    if (GameState *state = context.get_next_state())
        switch_state(state);

    if (context.has_exited())
        m_window.close();
}

void Game::switch_state(GameState *state)
{
    // If the state is a PauseMenu, BossMode or EndScreen it already handles the current state.
    if (dynamic_cast<PauseMenu *>(state) ||
        dynamic_cast<BossMode *>(state) ||
        dynamic_cast<EndScreen *>(state))
    {
        m_current_state = state;
        m_current_state->init(m_configuration);
    }
    else
    {
        delete m_current_state;
        m_current_state = state;
        m_current_state->init(m_configuration);
    }
}