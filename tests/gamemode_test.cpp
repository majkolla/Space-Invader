#include "gamestate.hpp"
#include "gameobject.hpp"

#include "player.hpp"

#include <iostream>

#include <catch.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/**
 * @brief Basic GamMode derived class for testing GameMode functions.
 *
 * @note All functions are public for testing purposes. They wrap the protected
 * functions of GameMode.
 */
class GameModeTest : public GameMode
{
public:
    GameModeTest(const std::vector<GameObject *> &objects) : GameMode{objects} {};
    virtual ~GameModeTest() = default;

    virtual void init(const GameConfiguration &) override
    {
        return;
    }

    void spawn_object(GameObject *object)
    {
        GameMode::spawn_object(object);
        return;
    }

    void set_player(Player *player)
    {
        GameMode::set_player(player);
        return;
    }

    void remove_player(bool delete_player)
    {
        GameMode::remove_player(delete_player);
        return;
    }

    Player *get_player() const
    {
        return GameMode::get_player();
    }

    void clear_objects(bool delete_player)
    {
        GameMode::clear_objects(delete_player);
        return;
    }

    void set_music(const std::string &path, float volume = 100.0f)
    {
        GameMode::set_music(path, volume);
        return;
    }

    void play_music()
    {
        GameMode::play_music();
        return;
    }

    void pause_music()
    {
        GameMode::pause_music();
        return;
    }

    void loop_music(bool loop)
    {
        GameMode::loop_music(loop);
        return;
    }

    void fade_out_music(float duration)
    {
        GameMode::fade_out_music(duration);
        return;
    }

    void fade_in_music(float duration, float target_volume = 100.0f)
    {
        GameMode::fade_in_music(duration, target_volume);
        return;
    }

    void set_music_volume(float volume)
    {
        GameMode::set_music_volume(volume);
        return;
    }

    const std::vector<GameObject *> &get_objects() const
    {
        return GameMode::get_objects();
    }

    sf::Music &get_music()
    {
        return GameMode::get_music();
    }
};

/**
 * @brief Basic GameObject derived class for testing GameMode functions.
 */
class TestObject : public GameObject
{
public:
    TestObject(unsigned int lifetime = 1, bool info = true)
        : m_id{object_count++},
          m_update_count{0},
          m_render_count{0},
          m_handle_count{0},
          m_lifetime{lifetime},
          m_info{info}
    {
        if (m_info)
            std::cout << "\tTestObject '" << m_id << "' created." << std::endl;
    }

    virtual ~TestObject() = default;
    virtual void update(Context &) override
    {
        if (m_info)
        {
            std::cout << "\tTestObject '" << m_id << "' updated. "
                      << "Frame: " << m_update_count << " Lifetime: " << m_lifetime
                      << std::endl;
        }

        if (m_update_count >= m_lifetime)
        {
            GameObject::remove();
        }
        m_update_count++;
        return;
    }

    virtual void render(sf::RenderWindow &) const override
    {
        if (m_info)
        {
            std::cout << "\tTestObject '" << m_id << "' rendered" << std::endl;
        }
        m_render_count++;
        return;
    }

    virtual bool handle(const sf::Event &event, Context &) override
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (m_info)
            {
                std::cout << "\tTestObject '" << m_id << "' handled key pressed event."
                          << std::endl;
            }
        }
        else
        {
            if (m_info)
                std::cout << "\tTestObject '" << m_id << "' handled" << std::endl;
        }
        m_handle_count++;
        return false;
    }

    virtual sf::FloatRect bounds() const override
    {
        return sf::FloatRect();
    }

    virtual void collision(const GameObject *) override
    {
        return;
    }

    static unsigned int object_count;

private:
    unsigned int m_id;
    unsigned int m_update_count;
    mutable unsigned int m_render_count; // Mutable because render is const.
    unsigned int m_handle_count;
    unsigned int m_lifetime;
    bool m_info;
};

unsigned int TestObject::object_count = 0;

/**
 * @brief Basic GameObject derived class for testing GameMode functions.
 *
 */
class SpawnerTestObject : public GameObject
{
public:
    SpawnerTestObject(unsigned int lifetime = 1, unsigned int spawn_lifetime = 1, bool info = true)
        : m_id{object_count++},
          m_update_count{0},
          m_render_count{0},
          m_handle_count{0},
          m_lifetime{lifetime},
          m_spawn_lifetime{spawn_lifetime},
          m_info(info)
    {
        if (m_info)
            std::cout << "\tSpawnerTestObject '" << m_id << "' created." << std::endl;
    }

    virtual ~SpawnerTestObject() = default;
    virtual void update(Context &c) override
    {
        if (m_info)
        {
            std::cout << "\tSpawnerTestObject '" << m_id << "' updated. "
                      << "Frame: " << m_update_count << " Lifetime: " << m_lifetime
                      << std::endl;
        }

        if (m_update_count >= m_lifetime)
        {
            GameObject::remove();
        }
        c.spawn_object(new TestObject{});
        m_update_count++;
        return;
    }

    virtual void render(sf::RenderWindow &) const override
    {
        if (m_info)
            std::cout << "\tSpawnerTestObject '" << m_id << "' rendered" << std::endl;
        m_render_count++;
        return;
    }

    virtual bool handle(const sf::Event &, Context &) override
    {
        if (m_info)
            std::cout << "\tSpawnerTestObject '" << m_id << "' handled" << std::endl;
        m_handle_count++;
        return false;
    }

    virtual sf::FloatRect bounds() const override
    {
        return sf::FloatRect();
    }

    virtual void collision(const GameObject *) override
    {
        return;
    }

    static unsigned int object_count;

private:
    unsigned int m_id;
    unsigned int m_update_count;
    mutable unsigned int m_render_count; // Mutable because render is const.
    unsigned int m_handle_count;
    unsigned int m_lifetime;
    unsigned int m_spawn_lifetime;
    bool m_info;
};

unsigned int SpawnerTestObject::object_count = 0;

class CollisionTestObject : public GameObject
{
public:
    CollisionTestObject(
        unsigned int lifetime = 1, const sf::Vector2f &position = {0.f, 0.f}, bool info = true)
        : m_id{object_count++},
          m_update_count{0},
          m_render_count{0},
          m_handle_count{0},
          m_lifetime{lifetime},
          m_position{position},
          m_info{info}
    {
        if (m_info)
            std::cout << "\tCollisionTestObject '" << m_id << "' created." << std::endl;
    }

    virtual ~CollisionTestObject() = default;

    virtual void update(Context &) override
    {
        if (m_info)
        {
            std::cout << "\tCollisionTestObject '" << m_id << "' updated. "
                      << "Frame: " << m_update_count << " Lifetime: " << m_lifetime
                      << std::endl;
        }

        if (m_update_count >= m_lifetime)
        {
            GameObject::remove();
        }
        m_update_count++;
        return;
    }

    virtual void render(sf::RenderWindow &) const override
    {
        if (m_info)
            std::cout << "\tCollisionTestObject '" << m_id << "' rendered" << std::endl;
        m_render_count++;
        return;
    }

    virtual bool handle(const sf::Event &, Context &) override
    {
        if (m_info)
            std::cout << "\tCollisionTestObject '" << m_id << "' handled" << std::endl;
        m_handle_count++;
        return false;
    }

    virtual sf::FloatRect bounds() const override
    {
        return sf::FloatRect{m_position, {1.f, 1.f}};
    }

    virtual void collision(const GameObject *object) override
    {
        if (const auto *obj = dynamic_cast<const CollisionTestObject *>(object))
        {
            if (m_info)
            {
                std::cout << "\tCollisionTestObject '" << m_id << "' collided "
                          << "with CollisionTestObject '" << obj->get_id() << "'." << std::endl;
            }
            remove();
        }
        return;
    }

    unsigned int get_id() const { return m_id; }

    static unsigned int object_count;

private:
    unsigned int m_id;
    unsigned int m_update_count;
    mutable unsigned int m_render_count; // Mutable because render is const.
    unsigned int m_handle_count;
    unsigned int m_lifetime;
    sf::Vector2f m_position;
    bool m_info;
};

unsigned int CollisionTestObject::object_count = 0;

/**
 * @brief Simulate a number of frames. Helper function for testing.
 *
 * @param gm game mode to simulate.
 * @param frames number of frames to simulate;
 */
void simulate_frames(GameMode &gm, unsigned int frames)
{
    // Create window and context. Needed to use GameMode functions.
    sf::RenderWindow window{};
    Context c{sf::Time::Zero, window};
    for (unsigned int i = 0; i < frames; i++)
    {
        gm.update(c);
        gm.render(window);
        gm.handle(sf::Event{}, c);
    }
}

/**
 * @brief Simulate fade in and fade out of music. Helper function for testing.
 *
 * @param gm game mode to simulate.
 * @param time time to fade in/out.
 */
void simulate_music_fade(GameModeTest &gm, float time = 1.f)
{
    sf::RenderWindow window{};
    Context c{sf::Time::Zero, window};
    std::cout << "Simulate fade in and fade out of music for " << time << " seconds each\n";
    gm.fade_in_music(time);
    sf::Music &music{gm.get_music()};
    while (music.getStatus() == sf::Music::Status::Playing)
    {
        gm.update(c);
        gm.fade_out_music(time);
    }
}

/**
 * @brief Return heap allocated player object. Helper function for testing.
 *
 * @return Player* pointer to heap allocated player object.
 */
Player *create_player()
{
    return new Player{{}, 0.f, 0.f};
}

TEST_CASE("Update / Render / Handle")
{
    // Create game mode with one object.
    std::cout << "Create game mode with one object and simulate two frames:\n";
    GameModeTest gm{{new TestObject{}}};
    CHECK(gm.get_objects().size() == 1); // One object created.
    simulate_frames(gm, 1);
    CHECK(gm.get_objects().size() == 1); // One object updated.
    simulate_frames(gm, 1);
    CHECK(gm.get_objects().size() == 0); // One object updated and removed (lifetime expired).
}

TEST_CASE("Spawning objects")
{
    // Create game mode with one object.
    std::cout << "\nCreate game mode with one spawner object and simulate four frames:\n";
    GameModeTest gm{{new SpawnerTestObject{}}};
    CHECK(gm.get_objects().size() == 1); // One spawner created object.
    simulate_frames(gm, 1);
    CHECK(gm.get_objects().size() == 2); // One spawner object and one spawned object.
    simulate_frames(gm, 1);
    CHECK(gm.get_objects().size() == 2); // Two spawned objects. (spawner lifetime expired in previous frame)
    simulate_frames(gm, 1);
    CHECK(gm.get_objects().size() == 1); // One spawned object  remaining (lifetime expired ).
    simulate_frames(gm, 1);
    CHECK(gm.get_objects().size() == 0); // No objects left (lifetime expired).
}

TEST_CASE("Collision")
{
    // Create game mode with two CollisionTestObject that collides.
    std::cout << "\nCreate game mode with two collision objects that collides and simulate two frames:\n";
    GameModeTest gm{{new CollisionTestObject{}, new CollisionTestObject{}}};
    CHECK(gm.get_objects().size() == 2); // Two created objects.
    simulate_frames(gm, 1);
    CHECK(gm.get_objects().size() == 0); // Both object removed (collision).
    simulate_frames(gm, 1);
    CHECK(gm.get_objects().size() == 0); // No objects removed. (removed in previous frame)

    // Create game mode with two CollisionTestObject that does not collide.
    std::cout << "\nCreate game mode with two collision objects that do not collide and simulate two frames:\n";
    GameModeTest gm2{{new CollisionTestObject{1, {100.f, 100.f}}, new CollisionTestObject{}}};
    CHECK(gm2.get_objects().size() == 2); // Two created objects.
    simulate_frames(gm2, 1);
    CHECK(gm2.get_objects().size() == 2); // No objects removed.
    simulate_frames(gm2, 1);
    CHECK(gm2.get_objects().size() == 0); // Both objects removed (lifetime expired)
}

TEST_CASE("Deletion of objects")
{
    GameModeTest gm{{}};
    // One object
    gm.spawn_object(new TestObject{1, false});
    CHECK(gm.get_objects().size() == 1);
    gm.clear_objects(true);
    CHECK(gm.get_objects().size() == 0);

    // Multiple (3) objects
    gm.spawn_object(new TestObject{1, false});
    gm.spawn_object(new TestObject{1, false});
    gm.spawn_object(new TestObject{1, false});
    CHECK(gm.get_objects().size() == 3);
    gm.clear_objects(true);
    CHECK(gm.get_objects().size() == 0);
}

TEST_CASE("Deletion of player")
{
    GameModeTest gm{{}};

    // Set player and delete it with clear_objects.
    {
        gm.set_player(create_player());
        CHECK(gm.get_objects().size() == 1);
        CHECK(gm.get_player() != nullptr);
        gm.clear_objects(true);
        CHECK(gm.get_player() == nullptr);
    }

    // Set player and delete it with remove_player.
    {
        gm.set_player(create_player());
        CHECK(gm.get_objects().size() == 1);
        CHECK(gm.get_player() != nullptr);
        gm.remove_player(true);
        CHECK(gm.get_objects().size() == 0);
        CHECK(gm.get_player() == nullptr);
    }

    // Set player and remove but do not delete it with remove_player.
    {
        
        Player * player{create_player()};
        gm.set_player(player);
        CHECK(gm.get_objects().size() == 1);
        CHECK(gm.get_player() != nullptr);
        gm.remove_player(false);
        CHECK(gm.get_objects().size() == 0);
        CHECK(gm.get_player() == nullptr);
        delete player; // Delete to prevent memory leak.
    }
}

TEST_CASE("Combination of player and objects")
{
    GameModeTest gm{{}};
    // Set player, spawn object and delete them with clear_objects.
    {
        gm.set_player(create_player());
        gm.spawn_object(new TestObject{1, false});
        CHECK(gm.get_objects().size() == 2);
        CHECK(gm.get_player() != nullptr);
        gm.clear_objects(true);
        CHECK(gm.get_objects().size() == 0);
        CHECK(gm.get_player() == nullptr);
    }

    // Set player, spawn object and delete object with clear_objects. Player should remain.
    {
        gm.set_player(create_player());
        gm.spawn_object(new TestObject{1, false});
        CHECK(gm.get_objects().size() == 2);
        CHECK(gm.get_player() != nullptr);
        gm.clear_objects(false);
        CHECK(gm.get_objects().size() == 1);
        CHECK(gm.get_player() != nullptr);
        gm.remove_player(true);
        CHECK(gm.get_objects().size() == 0);
        CHECK(gm.get_player() == nullptr);
    }

    // Set Player, spawn objects and delete player with remove_player.
    {
        gm.set_player(create_player());
        gm.spawn_object(new TestObject{1, false});
        gm.spawn_object(new TestObject{1, false});
        CHECK(gm.get_objects().size() == 3);
        CHECK(gm.get_player() != nullptr);
        gm.remove_player(true);
        CHECK(gm.get_objects().size() == 2);
        CHECK(gm.get_player() == nullptr);
    }
}

// This test will play music in different ways and check if the status is correct.
TEST_CASE("MUSIC")
{
    // Play music for one second check if correct status. (audio will play)
    GameModeTest gm{{}};
    gm.set_music("assets/sounds/normal_music.ogg");
    std::cout << "\nPlaying music for one second.\n";
    gm.play_music();
    CHECK(gm.get_music().getStatus() == sf::SoundSource::Status::Playing);
    sf::sleep(sf::seconds(1));
    gm.pause_music();
    CHECK(gm.get_music().getStatus() == sf::SoundSource::Status::Paused);
    simulate_music_fade(gm, 2.f);
    CHECK(gm.get_music().getStatus() == sf::SoundSource::Status::Paused);

    // Check if loop function works.
    gm.loop_music(true);
    CHECK(gm.get_music().getLoop() == true);
    gm.loop_music(false);
    CHECK(gm.get_music().getLoop() == false);

    // Check set volume function.
    gm.set_music_volume(50.f);
    CHECK(gm.get_music().getVolume() == 50.f);

    CHECK_THROWS(gm.set_music("XXXX"));
}