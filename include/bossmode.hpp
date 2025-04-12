#pragma once

#include "gamestate.hpp"
#include "gameobject.hpp"

class BossMode : public GameMode
{
public:
    BossMode(GameMode *previous_state, Player *current_player,
             unsigned int boss_level, unsigned int player_level);

    ~BossMode();

    BossMode(const BossMode &) = delete;
    BossMode &operator=(const BossMode &) = delete;

    void render(sf::RenderWindow &window) const override;
    void update(Context &context) override;
    void handle(const sf::Event &event, Context &context) override;
    void init(const GameConfiguration &gc) override;

private:
    GameObject* m_boss;
    bool m_to_normal;
    sf::Time m_to_normal_time;
    float m_time; 

    GameMode *m_previous_state;

    sf::Sprite m_scoreboard;
    sf::Text m_level_number_text;
    sf::Text m_player_score_text;
    sf::Text m_player_health_text;
    sf::Text m_boss_countdown_text;

    unsigned m_boss_level; 
    unsigned m_player_level; 

    void render_texts(sf::RenderWindow &window) const;
    void init_texts();
    void update_texts();

    void to_normal(Context &context);
    void to_end(Context &context);
};