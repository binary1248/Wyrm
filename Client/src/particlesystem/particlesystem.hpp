#ifndef PARTICLESYSTEM_HPP_INCLUDED
#define PARTICLESYSTEM_HPP_INCLUDED

#include <deque>
#include <memory>

#include <particlesystem/particle.hpp>
#include <particlesystem/particleemitter.hpp>

class ParticleSystem : public std::enable_shared_from_this<ParticleSystem> {
  public:
    ParticleSystem();
    ~ParticleSystem();

    void Draw( sf::RenderTarget& target );

    void Tick( float time );
    void Start( float time = 0.0f );
    void Stop();

    void AddEmitter( const ParticleEmitterPtr& emitter );
    void AddParticle( const ParticlePtr& particle );

    const sf::Vector2f& GetPosition() const;
    void SetPosition( const sf::Vector2f& position );

    const std::deque<ParticlePtr>& GetPool() const;

  private:
    sf::Vector2f m_position;

    std::deque<ParticleEmitterPtr> m_emitters;
    std::deque<ParticlePtr> m_particles;
    std::deque<ParticlePtr> m_particle_pool;

    bool m_running;
};

typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;

#endif // PARTICLESYSTEM_HPP_INCLUDED
