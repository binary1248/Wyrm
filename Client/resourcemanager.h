#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

struct particle_id {
  int size;
  float sigma2;
  float exp;
};

class ResourceManager {
  public:
    ResourceManager();
    ~ResourceManager();

    sf::Image* OpenImage(std::string);
    sf::SoundBuffer* OpenSound(std::string);
    sf::Image* GetParticle(int size, float sigma2, float exp);
    sf::Image* GetBackground(int width, int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes );
  private:
    std::map<std::string, sf::Image*> images;
    std::map<std::string, sf::SoundBuffer*> sounds;
    std::vector< std::pair<particle_id, sf::Image*> > particles;
};

#endif // RESOURCEMANAGER_H_INCLUDED
