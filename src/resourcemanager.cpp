#include <resourcemanager.hpp>
#include <map>
#include <sstream>

std::map<std::string, sf::Font> ResourceManager::Fonts{};
std::map<std::string, sf::Texture> ResourceManager::Textures{};
std::map<std::string, sf::SoundBuffer> ResourceManager::Sound_buffers{};
std::map<std::string, sf::Sound> ResourceManager::Sounds{};

sf::Texture &ResourceManager::load_texture(std::string const &path)
    // Inspired by lecture by Christoffer Holm. (https://www.ida.liu.se/~TDDC76/current/fo/index.sv.shtml)

{
    auto pair{Textures.find(path)};
    if (pair == end(Textures))
    {
        sf::Texture texture;
        if (!texture.loadFromFile(path))
        {
            std::stringstream ss;
            ss << "the file " << path << " was not loaded correctly!"; 
            throw std::logic_error(ss.str());
        }
        pair = Textures.insert({path, texture}).first;
    }
    return pair->second;
}

sf::Sound &ResourceManager::load_sound(std::string const &path)
{
    auto pair{Sounds.find(path)};
    if (pair == end(Sounds))
    {
        sf::SoundBuffer &sound_buffer{load_sound_buffer(path)};
        sf::Sound sound{sound_buffer};
        pair = Sounds.insert({path, sound}).first;
    }
    return pair->second;
}

sf::SoundBuffer &ResourceManager::load_sound_buffer(const std::string &path)
{
    auto pair{Sound_buffers.find(path)};
    if (pair == end(Sound_buffers))
    {
        sf::SoundBuffer sound_buffer;
        if (!sound_buffer.loadFromFile(path))
        {
            std::stringstream ss;
            ss << "the file " << path << " was not loaded correctly!"; 
            throw std::logic_error(ss.str());
        }
        pair = Sound_buffers.insert({path, sound_buffer}).first;
    }
    return pair->second;
}

sf::Font &ResourceManager::load_font(std::string const &path)
{
    auto pair{Fonts.find(path)};
    if (pair == end(Fonts))
    {
        sf::Font font;
        if (!font.loadFromFile(path))
        {
            std::stringstream ss;
            ss << "the file " << path << " was not loaded correctly!"; 
            throw std::logic_error(ss.str());
        }
        pair = Fonts.insert({path, font}).first;
    }
    return pair->second;
}
