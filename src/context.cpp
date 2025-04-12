#include "context.hpp"
#include <utility>
#include <sstream>

Context::Context(const sf::Time &delta, const sf::RenderWindow &window)
    : m_delta{delta},
      m_next_state{nullptr}, 
      m_new_objects{},
      m_window{window}, 
      m_quit{false}, 
      m_player{nullptr}
{
    m_new_objects.reserve(100);
}

Context::~Context() noexcept(false)
{
    if (!m_new_objects.empty())
    {
        std::stringstream ss{};
        ss << "ContextERROR: New objects is not zero. "
           << "Currently: "
           << m_new_objects.size();
        throw std::logic_error(ss.str());
    }
}

void Context::set_delta(const sf::Time &delta)
{
    m_delta = delta;
}

bool Context::set_next_state(GameState *next_state)
{
    if (m_next_state == nullptr)
    {
        m_next_state = next_state;
        return true;
    }
    return false;
}

void Context::spawn_object(GameObject *object)
{
    m_new_objects.push_back(object);
}

void Context::set_player(const Player *player)
{
    m_player = player;
}

const sf::Time &Context::get_delta() const
{
    return m_delta;
}

GameState *Context::get_next_state() const
{
    return m_next_state;
}

void Context::get_new_objects(std::vector<GameObject *> &objects)
{
    // Move objects into objects parameter & reset m_new_objects.
    objects = std::move(m_new_objects);
    m_new_objects = std::vector<GameObject *>{};
}

sf::Vector2u Context::get_window_size() const
{
    return m_window.getSize();
}

const sf::RenderWindow &Context::get_window() const
{
    return m_window;
}

const Player *Context::get_player() const
{
    return m_player;
}

void Context::exit()
{
    m_quit = true;
}

bool Context::has_exited() const
{
    return m_quit;
}