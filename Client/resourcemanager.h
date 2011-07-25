#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourceManager {
  public:
    ResourceManager();
    ~ResourceManager();

    sf::Image* OpenImage(std::string);
    sf::SoundBuffer* OpenSound(std::string);
  private:
    std::map<std::string, sf::Image*> images;
    std::map<std::string, sf::SoundBuffer*> sounds;
};

#endif // RESOURCEMANAGER_H_INCLUDED
