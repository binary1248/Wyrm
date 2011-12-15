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

struct GradientPoint {
	float position;

	unsigned char r, g, b, a;
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

    const TexturePtr GetBackground( int width, int height, int octaves, float frequency, float persistence,
		                                float contrast, float brightness,
		                                GradientPoint* gradient, std::size_t num_gradient_points );

    const TexturePtr GetPlanet( int size, int octaves, float frequency, float persistence,
		                            float contrast, float brightness,
		                            GradientPoint* gradient, std::size_t num_gradient_points );

  private:
    std::map<std::string, TexturePtr> m_textures;
    std::map<std::string, SoundBufferPtr> m_sounds;
    std::vector<ParticleTexturePair> m_particles;
};

typedef std::shared_ptr<ResourceManager> ResourceManagerPtr;

#endif // RESOURCEMANAGER_HPP_INCLUDED
