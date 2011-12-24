#ifndef SHIP_HPP_INCLUDED
#define SHIP_HPP_INCLUDED

#include <GL/gl.h>

#include <resourcemanager.hpp>
#include <objects/object.hpp>

class Ship : public Object {
  public:
    Ship( sf::Uint16 id, sf::Packet& packet );

    virtual ~Ship();

    virtual void Update( float time );

    virtual void Draw( sf::RenderWindow& target );

    virtual void HandlePacket( sf::Packet& packet );

    void SetPlayer( bool is_player );
    bool IsPlayer() const;

  private:
    void Init();

    sf::Vector2f m_acceleration;
    float m_thrust;

    sf::Sprite m_sprite;
    sf::Text m_text;

    float m_image_height;

		MeshPtr m_mesh;
    GLuint m_vertex_vbo;

    bool m_is_player;
};

typedef std::shared_ptr<Ship> ShipPtr;
typedef std::weak_ptr<Ship> ShipWeakPtr;

#endif // SHIP_HPP_INCLUDED
