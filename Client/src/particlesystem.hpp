#ifndef PARTICLESYSTEM_HPP_INCLUDED
#define PARTICLESYSTEM_HPP_INCLUDED

#include <vector>
#include <memory>

#include <GL/gl.h>

#include <SFML/Graphics.hpp>

typedef std::shared_ptr<sf::Texture> TexturePtr;

class ParticleSystem {
	public:
		struct Particle {
			struct Color {
				float r, g, b, a;
			};

			Particle();

			float life;
			float lifetime;

			sf::Vector2f position;
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

			float queue;
		};

		ParticleSystem();
		~ParticleSystem();

		ParticleSystem( const ParticleSystem& particle_system );

		void Draw( sf::RenderTarget& target );

		void Tick( float time );
		void Start( float time = 0.0f );
		void Stop();
		void StartEmitting();
		void StopEmitting();

		void AddEmitter( ParticleEmitter emitter );
		const std::vector<ParticleEmitter>& GetEmitters() const;

		const sf::Vector2f& GetPosition() const;
		void SetPosition( const sf::Vector2f& position );

		void SetTextureParameters( int size, float sigma2, float exp );

	private:
		struct ParticleVBOData {
			GLfloat r0, g0, b0, a0, u0, v0, x0, y0;
			GLfloat r1, g1, b1, a1, u1, v1, x1, y1;
			GLfloat r2, g2, b2, a2, u2, v2, x2, y2;
			GLfloat r3, g3, b3, a3, u3, v3, x3, y3;
		};

		sf::Vector2f m_position;

		std::vector<ParticleEmitter> m_emitters;
		std::vector<Particle> m_particles;

		std::vector<ParticleVBOData> m_particle_vbo_data;

		GLuint m_particles_vbo;

		TexturePtr m_texture;

		static std::mt19937 rng;

		bool m_running;
		bool m_emitting;
};

typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;

#endif // PARTICLESYSTEM_HPP_INCLUDED
