#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class Object {
  public:
		Object( sf::Uint16 type, sf::Uint16 id, sf::Packet& packet );

    virtual ~Object();

    virtual void Update( float time ) = 0;

    virtual void Draw( sf::RenderWindow& target );

    virtual void HandlePacket( sf::Packet& packet ) = 0;

    bool IsDeleted() const;
    void Delete();

    sf::Uint16 GetType() const ;
		void SetType( sf::Uint16 type );

		// ID Handlers
		sf::Uint16 GetID() const;
		void SetID( sf::Uint16 id );

		const sf::String& GetName() const;
		void SetName( const sf::String& name );

		// Attribute Handlers
		const sf::Vector2f& GetSize() const;
    void SetSize( const sf::Vector2f& size );

		const sf::Vector2f& GetPosition() const;
		void SetPosition( const sf::Vector2f& position );

		const sf::Vector2f& GetVelocity() const;
		void SetVelocity( const sf::Vector2f& velocity );

		float GetRotation() const;
		void SetRotation( float rotation );

		float GetRotationalVelocity() const;
		void SetRotationalVelocity( float rotational_velocity );

		// Resource Handlers
		void SetResourceId( sf::Uint32 id );
    sf::Uint32 GetResourceId() const;

  private:
		sf::Uint16 m_type;

    sf::Uint16 m_id;
    sf::String m_name;

    sf::Uint32 m_resource_id;

    sf::Vector2f m_size;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity;

    float m_rotation;
    float m_rotational_velocity;

    bool m_delete_me;
};

enum ServerToClientObject {
  OBJECT_UPDATE = 0,
  OBJECT_STATE,
  OBJECT_REMOVE
};

typedef std::shared_ptr<Object> ObjectPtr;
typedef std::weak_ptr<Object> ObjectWeakPtr;

#endif // OBJECT_HPP_INCLUDED
