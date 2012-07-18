#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

#include <map>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <particlesystem.hpp>

struct particle_id {
	int size;
	float max;
	float exp;
};

struct GradientPoint {
	float position;

	unsigned char r, g, b, a;
};

struct Mesh {
	struct Face {
		sf::Uint8 v0, v1, v2;
	};

	std::vector<sf::Vector3f> vertices;
	std::vector<Face> faces;
	std::vector<sf::Vector3f> normals;
};

typedef std::shared_ptr<sf::Texture> TexturePtr;
typedef std::shared_ptr<sf::SoundBuffer> SoundBufferPtr;
typedef std::shared_ptr<Mesh> MeshPtr;

typedef std::pair<particle_id, TexturePtr> ParticleTexturePair;

class ResourceManager {
	public:
		enum ResourceType {
			PLANE = 0,
			SPHERE = 1,
			MESH = 2,
			PARTICLESYSTEM = 3
		};

		ResourceManager();
		~ResourceManager();

		const TexturePtr OpenImage( std::string filename );

		const TexturePtr GetParticle( int size, float sigma2, float exp );

		const TexturePtr GetTexture( sf::Uint32 id ) const;

		const ParticleSystemPtr GetParticleSystem( sf::Uint32 id ) const;

		const MeshPtr GetMesh( sf::Uint32 id ) const;

		void GenerateResourceFromPacket( sf::Packet& packet );

		void GeneratePlaneTexture( sf::Packet& packet );

		void GeneratePlaneTexture( sf::Uint32 id, int width, int height, int octaves, float frequency, float persistence,
		                           float contrast, float brightness,
		                           GradientPoint* gradient, std::size_t num_gradient_points );

		void GenerateSphereTexture( sf::Packet& packet );

		void GenerateSphereTexture( sf::Uint32 id, int size, int octaves, float frequency, float persistence,
		                            float contrast, float brightness,
		                            GradientPoint* gradient, std::size_t num_gradient_points );

		void GenerateParticleSystem( sf::Packet& packet );

		void GenerateMesh( sf::Packet& packet );

	private:
		std::map<sf::Uint32, TexturePtr> m_textures;
		std::map<sf::Uint32, SoundBufferPtr> m_sounds;
		std::vector<ParticleTexturePair> m_particles;
		std::map<sf::Uint32, ParticleSystemPtr> m_particle_systems;
		std::map<sf::Uint32, MeshPtr> m_meshes;
};

typedef std::shared_ptr<ResourceManager> ResourceManagerPtr;

#endif // RESOURCEMANAGER_HPP_INCLUDED
