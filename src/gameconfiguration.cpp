#include "gameconfiguration.hpp"

#include <fstream>
#include <sstream>
#include <limits>
#include <iostream>

/*============================== EnemyMinionData==============================*/

EnemyMinionData::EnemyMinionData()
    : base_health{0},
      base_speed{0.0f},
      base_attack_cooldown{0.0f},
      base_projectile_speed{0.0f},
      base_projectile_prob{0.0f},
      base_powerup_prob{0.0f}
{
}

bool EnemyMinionData::set_value(const std::string &key, const std::string &value)
{
    std::stringstream ss{value};
    if (key == "BASE_HEALTH")
    {
        if (!(ss >> base_health))
            throw std::logic_error("BASE_HEALTH value is not a valid int.");
        return true;
    }
    else if (key == "BASE_SPEED")
    {
        if (!(ss >> base_speed))
            throw std::logic_error("BASE_SPEED value is not a valid float.");
        return true;
    }
    else if (key == "BASE_ATTACK_COOLDOWN")
    {
        if (!(ss >> base_attack_cooldown))
            throw std::logic_error("BASE_ATTACK_COOLDOWN value is not a valid float.");
        return true;
    }
    else if (key == "BASE_PROJECTILE_SPEED")
    {
        if (!(ss >> base_projectile_speed))
            throw std::logic_error("BASE_PROJECTILE_SPEED value is not a valid float.");
        return true;
    }
    else if (key == "BASE_PROJECTILE_PROB")
    {
        if (!(ss >> base_projectile_prob))
            throw std::logic_error("BASE_PROJECTILE_PROB value is not a valid float.");
        return true;
    }
    else if (key == "BASE_POWERUP_PROB")
    {
        if (!(ss >> base_powerup_prob))
            throw std::logic_error("BASE_POWERUP_PROB value is not a valid float.");
        return true;
    }
    return false;
}

/*==============================EnemyBossData=================================*/

EnemyBossData::EnemyBossData()
    : base_health{0},
      base_attack_cooldown{0.0f},
      base_projectile_speed{0.0f},
      base_attack_time{0.0f},
      base_attack_length{0}
{
}

bool EnemyBossData::set_value(const std::string &key, const std::string &value)
{
    std::stringstream ss{value};
    if (key == "BOSS_BASE_HEALTH")
    {
        if (!(ss >> base_health))
            throw std::logic_error("BASE_HEALTH value is not a valid int.");
        return true;
    }
    else if (key == "BOSS_BASE_ATTACK_COOLDOWN")
    {
        if (!(ss >> base_attack_cooldown))
            throw std::logic_error("BASE_ATTACK_COOLDOWN value is not a valid float.");
        return true;
    }
    else if (key == "BOSS_BASE_PROJECTILE_SPEED")
    {
        if (!(ss >> base_projectile_speed))
            throw std::logic_error("BASE_PROJECTILE_SPEED value is not a valid float.");
        return true;
    }
    else if (key == "BOSS_BASE_ATTACK_TIME")
    {
        if (!(ss >> base_attack_time))
            throw std::logic_error("ATTACK_TIME value is not a valid int.");
        return true;
    }
    else if (key == "BOSS_BASE_ATTACK_LENGTH")
    {
        if (!(ss >> base_attack_length))
            throw std::logic_error("ATTACK_LENGTH value is not a valid int.");
        return true;
    }
    return false;
}

/*===============================PlayerData===================================*/

PlayerData::PlayerData()
    : health{0},
      speed{0.0f},
      attack_cooldown{0.0f},
      projectile_speed{0.0f}
{
}

bool PlayerData::set_value(const std::string &key, const std::string &value)
{
    std::stringstream ss{value};
    if (key == "PLAYER_HEALTH")
    {
        if (!(ss >> health))
            throw std::logic_error("HEALTH value is not a valid int.");
        return true;
    }
    else if (key == "PLAYER_SPEED")
    {
        if (!(ss >> speed))
            throw std::logic_error("SPEED value is not a valid float.");
        return true;
    }
    else if (key == "PLAYER_PROJECTILE_SPEED")
    {
        if (!(ss >> projectile_speed))
            throw std::logic_error("PROJECTILE_SPEED value is not a valid float.");
        return true;
    }
    else if (key == "PLAYER_ATTACK_COOLDOWN")
    {
        if (!(ss >> attack_cooldown))
            throw std::logic_error("ATTACK_COOLDOWN value is not a valid float.");
        return true;
    }
    return false;
}

/*==============================NormalModeData================================*/

NormalModeData::NormalModeData()
    : start_level{0},
      level_increase_time{0.0f},
      boss_spawn_time{0.0f},
      base_spawn_time{0.0f},
      spawn_time_multiplier{0.0f},
      spawn_time_min{0.0f},
      spawn_seed{0},
      minion_data{},
      player_data{}
{
}

bool NormalModeData::set_value(const std::string &key, const std::string &value)
{
    std::stringstream ss{value};
    if (key == "START_LEVEL")
    {
        if (!(ss >> start_level))
            throw std::logic_error("START_LEVEL value is not a valid uint.");
        return true;
    }
    else if (key == "LEVEL_INCREASE_TIME")
    {
        if (!(ss >> level_increase_time))
            throw std::logic_error("LEVEL_INCREASE_TIME value is not a valid float.");
        return true;
    }
    else if (key == "BOSS_SPAWN_TIME")
    {
        if (!(ss >> boss_spawn_time))
            throw std::logic_error("BOSS_SPAWN_TIME value is not a valid float.");
        return true;
    }
    else if (key == "BASE_SPAWN_TIME")
    {
        if (!(ss >> base_spawn_time))
            throw std::logic_error("BASE_SPAWN_TIME value is not a valid float.");
        return true;
    }
    else if (key == "SPAWN_TIME_MULTIPLIER")
    {
        if (!(ss >> spawn_time_multiplier))
            throw std::logic_error("SPAWN_TIME_MULTIPLIER value is not a valid float.");
        return true;
    }
    else if (key == "SPAWN_TIME_MIN")
    {
        if (!(ss >> spawn_time_min))
            throw std::logic_error("SPAWN_TIME_MIN value is not a valid float.");
        return true;
    }
    else if (key == "SPAWN_SEED")
    {
        if (!(ss >> spawn_seed))
            throw std::logic_error("SPAWN_SEED value is not a valid uint.");
        return true;
    }
    else
    {
        if (minion_data.set_value(key, value))
            return true;
        if (player_data.set_value(key, value))
            return true;
    }
    return false;
}

/*==============================BossModeData==================================*/

BossModeData::BossModeData()
    : grace_period{0.0f},
      boss_pos_x{0.0f},
      boss_pos_y{0.0f},
      boss_data{}
{
}

bool BossModeData::set_value(const std::string &key, const std::string &value)
{
    std::stringstream ss{value};
    if (key == "BOSS_POS_X")
    {
        if (!(ss >> boss_pos_x))
            throw std::logic_error("BOSS_POS_X value is not a valid float.");
        return true;
    }
    else if (key == "BOSS_POS_Y")
    {
        if (!(ss >> boss_pos_y))
            throw std::logic_error("BOSS_POS_Y value is not a valid float.");
        return true;
    }
    else if (key == "GRACE_PERIOD")
    {
        if (!(ss >> grace_period))
            throw std::logic_error("GRACE_PERIOD value is not a valid float.");
        return true;
    }
    else
    {
        if (boss_data.set_value(key, value))
            return true;
    }
    return false;
}

/*=================================GameData===================================*/

GameData::GameData()
    : title{"Untitled"},
      window_width{750},
      window_height{750},
      fps{0},
      music_volume{0.0f},
      effects_volume{0.0f}
{
}

bool GameData::set_value(const std::string &key, const std::string &value)
{
    std::stringstream ss{value};
    if (key == "TITLE")
    {
        title = value;
        return true;
    }
    else if (key == "WINDOW_WIDTH")
    {
        if (!(ss >> window_width))
            throw std::logic_error("WINDOW_WIDTH value is not a valid uint.");
        return true;
    }
    else if (key == "WINDOW_HEIGHT")
    {
        if (!(ss >> window_height))
            throw std::logic_error("WINDOW_HEIGHT value is not a valid uint.");
        return true;
    }
    else if (key == "FPS")
    {
        if (!(ss >> fps))
            throw std::logic_error("FPS value is not a valid uint.");
        return true;
    }
    else if (key == "MUSIC_VOLUME")
    {
        if (!(ss >> music_volume))
            throw std::logic_error("MUSIC_VOLUME value is not a valid float.");
        return true;
    }
    else if (key == "EFFECTS_VOLUME")
    {
        if (!(ss >> effects_volume))
            throw std::logic_error("EFFECTS_VOLUME value is not a valid float.");
        return true;
    }
    return false;
}

/*==============================GameConfiguration=============================*/

GameConfiguration::~GameConfiguration()
{
    write_high_score(m_high_score);
}

const GameData &GameConfiguration::get_data() const
{
    return m_g_data;
}

const NormalModeData &GameConfiguration::get_normal_mode_data() const
{
    return m_nm_data;
}

const BossModeData &GameConfiguration::get_boss_mode_data() const
{
    return m_bm_data;
}

sf::Vector2u GameConfiguration::get_window_size() const
{
    return sf::Vector2u{m_g_data.window_width, m_g_data.window_height};
}

unsigned int GameConfiguration::get_high_score() const
{
    return m_high_score;
}

void GameConfiguration::update_high_score(unsigned int score) const
{
    score > m_high_score ? m_high_score = score : m_high_score = m_high_score;
}

void GameConfiguration::update_window_size(const sf::Vector2u &window_size)
{
    m_g_data.window_width = window_size.x;
    m_g_data.window_height = window_size.y;
}

GameConfiguration GameConfiguration::from_file(const std::string &path)
{
    std::ifstream file{path};
    if (!file.is_open())
    {
        return default_config();
    }

    GameData g_data{};
    NormalModeData nm_data{};
    BossModeData bm_data{};
    std::string line{}, key{}, value{};
    while (std::getline(file, line))
    {
        if (line.rfind("//", 0) == 0)
            continue;
        std::stringstream ss{line};
        std::getline(ss, key, '=');
        std::getline(ss, value, ';');
        check_key(key, value, g_data, nm_data, bm_data);
    }
    return GameConfiguration{g_data, nm_data, bm_data, load_high_score()};
}

GameConfiguration GameConfiguration::default_config()
{
    write_default_config();
    return from_file("config.txt");
}

void GameConfiguration::check_key(
    const std::string &key,
    const std::string &value,
    GameData &g_data,
    NormalModeData &nm_data,
    BossModeData &bm_data)
{
    if (g_data.set_value(key, value))
        return;
    if (nm_data.set_value(key, value))
        return;
    if (bm_data.set_value(key, value))
        return;
    std::cout << key << " ... " << value << std::endl;
}

void GameConfiguration::write_default_config()
{
    std::ofstream file{"config.txt"};
    if (!file.is_open())
        throw std::logic_error(
            "GameConfiguration::write_default_config: Could not write file.");

    // Write a default config file
    file << "// This is the configuration file for the game.\n"
         << "// Window related configuration. Window width and height should be greater than 750 pixels\n"
         << "TITLE=Space Invader;\n"
         << "WINDOW_WIDTH=900;\n"
         << "WINDOW_HEIGHT=900;\n"
         << "FPS=60;\n"
         << "// NormalMode related configuration\n"
         << "START_LEVEL=1;\n"
         << "LEVEL_INCREASE_TIME=10.0;\n"
         << "BOSS_SPAWN_TIME=45.0;\n"
         << "BASE_SPAWN_TIME=3.5;\n"
         << "SPAWN_TIME_MULTIPLIER=0.98;\n"
         << "SPAWN_TIME_MIN=0.3;\n"
         << "SPAWN_SEED=0;\n"
         << "// Player data\n"
         << "PLAYER_HEALTH=5;\n"
         << "PLAYER_SPEED=100.0;\n"
         << "PLAYER_PROJECTILE_SPEED=150.0;\n"
         << "PLAYER_ATTACK_COOLDOWN=0.75;\n"
         << "// Enemy minion data. Probability values should be between 0.0-1.0\n"
         << "BASE_HEALTH=1;\n"
         << "BASE_SPEED=25.0;\n"
         << "BASE_ATTACK_COOLDOWN=3.0;\n"
         << "BASE_PROJECTILE_SPEED=100.f\n"
         << "BASE_PROJECTILE_PROB=0.5;\n"
         << "BASE_POWERUP_PROB=0.25;\n"
         << "// BossMode related configuration\n"
         << "GRACE_PERIOD=15.0;\n"
         << "BOSS_POS_X=450.0;\n"
         << "BOSS_POS_Y=150.0;\n"
         << "// Enemy boss data\n"
         << "BOSS_BASE_HEALTH=40;\n"
         << "BOSS_BASE_ATTACK_COOLDOWN=1.0;\n"
         << "BOSS_BASE_PROJECTILE_SPEED=100.0;\n"
         << "BOSS_BASE_ATTACK_TIME=0.1;\n"
         << "BOSS_BASE_ATTACK_LENGTH=10\n"
         << "// Music and effects volume\n"
         << "MUSIC_VOLUME=50.0;\n"
         << "EFFECTS_VOLUME=100.0;\n";
}

unsigned int GameConfiguration::load_high_score()
{
    std::ifstream fs{"high_score.txt"};
    if (!fs.is_open())
    {
        return 0;
    }
    else
    {
        unsigned int high_score{};
        fs >> high_score;
        fs.close();
        return high_score;
    }
}

void GameConfiguration::write_high_score(unsigned int score)
{
    std::ofstream fs{"high_score.txt"};
    fs << score;
    fs.close();
}

GameConfiguration::GameConfiguration(
    const GameData &g_data,
    const NormalModeData &nm_data,
    const BossModeData &bm_data,
    unsigned int high_score)
    : m_g_data{g_data},
      m_nm_data{nm_data},
      m_bm_data{bm_data},
      m_high_score{high_score}
{
}