#ifndef PARTICLE_HPP_INCLUDED
#define PARTICLE_HPP_INCLUDED

#include <SFML/Graphics.hpp>

#include <resourcemanager.hpp>

class Particle {
  public:
    Particle( TexturePtr texture );
    Particle( const std::shared_ptr<Particle>& particle );
    ~Particle();

    Particle& operator=( const Particle& particle );

    void Tick( float time );

    void Draw( sf::RenderTarget& target );

    void SetVisible( bool visible );
    void Kill();
    bool IsAlive() const;

    float GetLifetime() const;
    void SetLifetime( float lifetime );

    void SetSizeStart( const sf::Vector2f& size );
    void SetSizeEnd( const sf::Vector2f& size );

    void SetColorStart( const sf::Color& color );
    void SetColorEnd( const sf::Color& color );

    void SetPosition( const sf::Vector2f& position );

    const sf::Vector2f& GetVelocity() const;
    void SetVelocity( const sf::Vector2f& velocity );

    const sf::Vector2f& GetAcceleration() const;
    void SetAcceleration( const sf::Vector2f& acceleration );

    void SetTexture( const TexturePtr& texture );

  private:
    TexturePtr m_texture;

    float m_life;
    float m_lifetime;

    sf::Vector2f m_size_start;
    sf::Vector2f m_size_end;
    sf::Vector2f m_size_step;

		sf::Color m_color;
    sf::Color m_color_start;
    sf::Color m_color_end;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Vector2f m_acceleration;

    sf::Sprite m_sprite;

    bool m_visible;
};

typedef std::shared_ptr<Particle> ParticlePtr;

#endif // PARTICLE_HPP_INCLUDED
