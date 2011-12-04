#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

#include <map>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

struct particle_id {
  int size;
  float max;
  float exp;
};

typedef std::shared_ptr<sf::Texture> TexturePtr;
typedef std::shared_ptr<sf::SoundBuffer> SoundBufferPtr;

typedef std::pair<particle_id, TexturePtr> ParticleTexturePair;

class ResourceManager {
  public:
    ResourceManager();
    ~ResourceManager();

    const TexturePtr OpenImage( std::string filename );
    const SoundBufferPtr OpenSound( std::string filename );

    const TexturePtr GetParticle( int size, float sigma2, float exp );
    const TexturePtr GetBackground( unsigned int width, unsigned int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes );
    const TexturePtr GetPlanet( unsigned int width, unsigned int height, const sf::Color& base_color, int seed, size_t num_octaves, float* amplitudes );

  private:
    std::map<std::string, TexturePtr> m_textures;
    std::map<std::string, SoundBufferPtr> m_sounds;
    std::vector<ParticleTexturePair> m_particles;
};

typedef std::shared_ptr<ResourceManager> ResourceManagerPtr;

#endif // RESOURCEMANAGER_HPP_INCLUDED
