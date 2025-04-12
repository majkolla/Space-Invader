#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// TODO: Add more configuration options.

struct EnemyMinionData
{
    int base_health;
    float base_speed;
    float base_attack_cooldown;
    float base_projectile_speed;
    float base_projectile_prob;
    float base_powerup_prob;

    /**
     * @brief Construct a new Enemy Minion Data object with default values.
     */
    EnemyMinionData();

    /**
     * @brief Set value of in struct based on given key and value. If key is not
     * recognized, false is returned. If key is recognized but value is invalid,
     * exception is thrown.
     *
     * @param key key to set value for.
     * @param value value to set.
     */
    bool set_value(const std::string &key, const std::string &value);
};

struct EnemyBossData
{
    int base_health;
    float base_attack_cooldown;
    float base_projectile_speed;
    float base_attack_time;
    int base_attack_length;

    /**
     * @brief Construct a new Enemy Boss Data object with default values.
     */
    EnemyBossData();

    /**
     * @brief Set value of in struct based on given key and value. If key is not
     * recognized, false is returned. If key is recognized but value is invalid,
     * exception is thrown.
     *
     * @param key key to set value for.
     * @param value value to set.
     */
    bool set_value(const std::string &key, const std::string &value);
};

struct PlayerData
{
    int health;
    float speed;
    float attack_cooldown;
    float projectile_speed;

    /**
     * @brief Construct a new Enemy Boss Data object with default values.
     */
    PlayerData();

    /**
     * @brief Set value of in struct based on given key and value. If key is not
     * recognized, false is returned. If key is recognized but value is invalid,
     * exception is thrown.
     *
     * @param key key to set value for.
     * @param value value to set.
     */
    bool set_value(const std::string &key, const std::string &value);
};

/**
 * @brief Struct containing data for normal mode.
 */
struct NormalModeData
{
    unsigned int start_level;
    float level_increase_time;
    float boss_spawn_time;
    float base_spawn_time;
    float spawn_time_multiplier;
    float spawn_time_min;
    unsigned int spawn_seed;

    EnemyMinionData minion_data;
    PlayerData player_data;

    /**
     * @brief Construct a new Normal Mode Data object with default values.
     */
    NormalModeData();

    /**
     * @brief Set value of in struct based on given key and value. If key is not
     * recognized, false is returned. If key is recognized but value is invalid,
     * exception is thrown.
     *
     * @param key key to set value for.
     * @param value value to set.
     */
    bool set_value(const std::string &key, const std::string &value);
};

/**
 * @brief Struct containing configuration data for the boss mode.
 */
struct BossModeData
{
    float grace_period;
    float boss_pos_x;
    float boss_pos_y;

    EnemyBossData boss_data;

    /**
     * @brief Construct a new Boss Mode Data object with default values.
     */
    BossModeData();

    /**
     * @brief Set value of in struct based on given key and value. If key is not
     * recognized, false is returned. If key is recognized but value is invalid,
     * exception is thrown.
     *
     * @param key key to set value for.
     * @param value value to set.
     */
    bool set_value(const std::string &key, const std::string &value);
};

/**
 * @brief Struct containing all the data needed to configure the game.
 */
struct GameData
{
    std::string title;
    unsigned int window_width;
    unsigned int window_height;
    unsigned int fps;
    float music_volume;
    float effects_volume;

    /**
     * @brief Construct a new Game Data object with default values.
     */
    GameData();

    /**
     * @brief Set value of in struct based on given key and value. If key is not
     * recognized, false is returned. If key is recognized but value is invalid,
     * exception is thrown.
     *
     * @param key key to set value for.
     * @param value value to set.
     */
    bool set_value(const std::string &key, const std::string &value);
};

/**
 * @brief Class containing game configuration. Is needed to create a Game object.
 * Can only be created from a file. A file, config.txt, is created if the given
 * file does not exist.
 *
 * @details This is essentially a wrapper around the Data structs above, making all
 * the data private and providing getters and setters (if needed). Also provides a
 * static method to create a GameConfiguration object from a file.
 *
 * File format is as follows:
 *          // This is a comment
 *          [KEY0]=[VALUE0];
 *          [KEY1]=[VALUE1];
 *
 * Keys are:
 *      TITLE (string), WINDOW_WIDTH (int), WINDOW_HEIGHT (uint), FPS (uint),
 *      START_LEVEL (uint), LEVEL_INCREASE_TIME (float), BOSS_SPAWN_TIME (float),
 *      BASE_SPAWN_TIME (float), SPAWN_TIME_MULTIPLIER (float),
 *      SPAWN_TIME_MIN (float), SPAWN_SEED (uint), PLAYER_HEALTH (int),
 *      PLAYER_SPEED (float), PLAYER_PROJECTILE_SPEED (float),
 *      PLAYER_ATTACK_COOLDOWN (float), BASE_HEALTH (int), BASE_SPEED (float),
 *      BASE_ATTACK_COOLDOWN (float), BASE_PROJECTILE_SPEED (float),
 *      BASE_PROJECTILE_PROB (float), BASE_POWERUP_PROB (float), GRACE_PERIOD (float),
 *      BOSS_POS_X (float), BOSS_POS_Y (float), BOSS_BASE_HEALTH (int),
 *      BOSS_BASE_ATTACK_COOLDOWN (float), BOSS_BASE_PROJECTILE_SPEED (float),
 *      BOSS_BASE_ATTACK_TIME (float), BOSS_BASE_ATTACK_LENGTH (int),
 *      MUSIC_VOLUME (float), EFFECTS_VOLUME (float)
 *
 * @note If types of values are invalid, a std::logic_error is thrown. Keys are case
 * sensitive and unkown keys are ignored. If a key is missing, a default value
 * is used, which most likely will not be what you want.
 */
class GameConfiguration
{
public:
    ~GameConfiguration();

    /**
     * @brief Get underlying GameData struct.
     *
     * @return const GameData& struct will all the data.
     */
    const GameData &get_data() const;

    /**
     * @brief Get underlying NormalModeData struct.
     *
     * @return const NormalModeData& struct will all the data.
     */
    const NormalModeData &get_normal_mode_data() const;

    /**
     * @brief Get underlying BossModeData struct.
     *
     * @return const BossModeData& struct will all the data.
     */
    const BossModeData &get_boss_mode_data() const;

    /**
     * @brief Get window size vector. Width in x and height in y.
     *
     * @return sf::Vector2u
     */
    sf::Vector2u get_window_size() const;

    /**
     * @brief Update window size vector. Width in x and height in y.
     *
     * @note This does not change the actual window size, only the value returned by
     * get_window_size(). Might be needed if the window is resized by the user.
     *
     * @param window_size new window size.
     */
    void update_window_size(const sf::Vector2u &window_size);

    /**
     * @brief Get current high score.
     */
    unsigned int get_high_score() const;

    /**
     * @brief Set the new high score. If the new score is lower than the old one,
     * nothing happens.
     *
     * @param score new high score.
     */
    void update_high_score(unsigned int score) const;

    /**
     * @brief Load GameConfig from file.
     *
     * @param path path to config file.
     * @return GameConfiguration, configuration based on loaded file.
     */
    static GameConfiguration from_file(const std::string &path = "config.txt");

    /**
     * @brief Default configuration. Will first write a default config file to
     * config.txt and then load it. Will overwrite any existing config.txt file.
     *
     * @return GameConfiguration a default configuration.
     */
    static GameConfiguration default_config();

private:
    GameData m_g_data;
    NormalModeData m_nm_data;
    BossModeData m_bm_data;
    mutable unsigned int m_high_score;

    /**
     * @brief Check if the read config key is valid. If valid, set the value in
     * the correct config data. Will throw exception if type of value for given
     * key is invalid.
     *
     * @param key string key.
     * @param value value for the given key.
     * @param g_data[in, out] config data to set the value in.
     * @param nm_data[in, out] normal mode data to set the value in.
     * @param bm_data[in, out] boss mode data to set the value in.
     */
    static void check_key(
        const std::string &key,
        const std::string &value,
        GameData &g_data,
        NormalModeData &nm_data,
        BossModeData &bm_data);

    /**
     * @brief Write default config to file "config.txt". Will be called if the
     * file is not found when using the from_file(...) method.
     */
    static void write_default_config();

    /**
     * @brief Get the current high score from the high score file (high_score.txt).
     * If the file does not exist, a default value of 0 is returned.
     */
    static unsigned int load_high_score();

    /**
     * @brief Write the current high score to the high score file (high_score.txt).
     *
     * @param score score to write.
     */
    static void write_high_score(unsigned int score);

    /**
     * @brief Construct a new Game Configuration object based on the given data.
     *
     * @param g_data game configuration data.
     */
    GameConfiguration(
        const GameData &g_data,
        const NormalModeData &nm_data,
        const BossModeData &bm_data,
        unsigned int high_score);
};