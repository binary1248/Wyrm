#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

#include <map>
#include <memory>

#include <networkmanager.hpp>

struct GradientPoint {
	float position;

	unsigned char r, g, b, a;
};

struct ParticleSystem {
	struct Particle {
		struct Color {
			float r, g, b, a;
		};

		float lifetime;

		sf::Vector2f velocity;
		sf::Vector2f acceleration;

		sf::Vector2f size_start;
		sf::Vector2f size_end;

		Particle::Color color_start;
		Particle::Color color_end;
	};

	struct ParticleEmitter {
		Particle prototype;

		float rate;
		float spread;
		float radius_start;
		float radius_end;
	};

	std::vector<ParticleEmitter> emitters;

	int texture_size;
	float texture_sigma2;
	float texture_exp;
};

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

		void Tick( float time );

		void FillResourcePacket( sf::Uint32 id, PacketPtr packet ) const;

		void AddPlaneTexture( sf::Uint32 id, int width, int height, int octaves, float frequency, float persistence,
		                      float contrast, float brightness,
		                      GradientPoint* gradient, std::size_t num_gradient_points );

		void AddSphereTexture( sf::Uint32 id, int size, int octaves, float frequency, float persistence,
		                       float contrast, float brightness,
		                       GradientPoint* gradient, std::size_t num_gradient_points );

		void AddParticleSystem( sf::Uint32 id, ParticleSystem particle_system );

		void AddMesh( sf::Uint32 id, std::string mesh_file );

	private:
		void LoadResources();

		std::map<sf::Uint32, const PacketPtr> m_resources;

		bool m_resources_loaded;
};

typedef std::shared_ptr<ResourceManager> ResourceManagerPtr;

#endif // RESOURCEMANAGER_HPP_INCLUDED
