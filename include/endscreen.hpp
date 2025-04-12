#pragma once

#include "gamestate.hpp"
#include "gameobject.hpp"

class EndScreen : public Menu
{
public:
    EndScreen(GameState *previous_state, Player *current_player);
    virtual ~EndScreen();
    EndScreen(const EndScreen &) = delete;
    EndScreen &operator=(const EndScreen &) = delete;

    void render(sf::RenderWindow &window) const override;
    void update(Context &context) override;
    void handle(const sf::Event &event, Context &context) override;
    void init(const GameConfiguration &gc) override;

private:
    GameState *m_previous_state;
    Player *m_current_player;
    sf::Sprite m_scoreboard;
    
    sf::Text m_player_score_text; 
    sf::Text m_death_text; 
    sf::Text m_highscore_text;
    unsigned int m_current_highscore;

    void init_texts(const GameConfiguration &gc); 
    void render_texts(sf::RenderWindow &window) const; 
    void update_texts();
    
    void check_highscore(const GameConfiguration &gc);
    void set_new_highscore(const GameConfiguration &gc);
    bool m_new_highscore;

};

