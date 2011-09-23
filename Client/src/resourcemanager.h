#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

struct particle_id {
  int size;
  float max;
  float exp;
};

typedef boost::shared_ptr<sf::Texture> TexturePtr;
typedef boost::shared_ptr<sf::SoundBuffer> SoundBufferPtr;

typedef std::pair<particle_id, TexturePtr> ParticleTexturePair;

class ResourceManager {
  public:
    ResourceManager();
    ~ResourceManager();

    TexturePtr OpenImage(std::string);
    SoundBufferPtr OpenSound(std::string);
    TexturePtr GetParticle(int size, float sigma2, float exp);
    TexturePtr GetBackground(int width, int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes );
    TexturePtr GetPlanet(int width, int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes );
  private:
    std::map<std::string, TexturePtr> textures;
    std::map<std::string, SoundBufferPtr> sounds;
    std::vector<ParticleTexturePair> particles;
};

typedef boost::shared_ptr<ResourceManager> ResourceManagerPtr;

#endif // RESOURCEMANAGER_H_INCLUDED
