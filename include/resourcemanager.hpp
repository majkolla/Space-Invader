#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

class ResourceManager // Texture, font, sound
{
public:
    ResourceManager();
    ~ResourceManager();

    static sf::Texture &load_texture(std::string const &path);
    static sf::Sound &load_sound(std::string const &path);
    static sf::SoundBuffer &load_sound_buffer(std::string const &path);
    static sf::Font &load_font(std::string const &path);

private:
    static std::map<std::string, sf::Texture> Textures;
    static std::map<std::string, sf::Font> Fonts;
    static std::map<std::string, sf::SoundBuffer> Sound_buffers;
    static std::map<std::string, sf::Sound> Sounds;
};
