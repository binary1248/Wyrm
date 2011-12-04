#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <set>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <networkmanager.hpp>

class Object {
  public:
    Object( sf::Uint16 type, sf::String name, sf::Vector2f position,
						sf::Vector2f velocity, float rotation, float rotational_velocity );
    virtual ~Object();

    virtual void Update( float time );

    // Network Handlers
    virtual void FillPartialPacket( const PacketPtr& packet );
    virtual void FillFullPacket( const PacketPtr& packet ) = 0;
    virtual void HandlePacket( const PacketPtr& packet ) = 0;

    // ID Handlers
    sf::Uint16 GetId() const;
    sf::Uint16 GetType() const;
    void SetName( const sf::String& name );
    const sf::String& GetName() const;

    // Dirty flag Handlers
    bool IsDirty() const;
    void SetDirty();
    void ClearDirty();

    bool IsFresh() const;
    void SetFresh();
    void ClearFresh();

    // Delete Handlers
    void Delete();
    bool IsDeleted() const;

    const sf::Vector2f& GetPosition() const;
    void SetPosition( const sf::Vector2f& position );

		const sf::Vector2f& GetVelocity() const;
		void SetVelocity( const sf::Vector2f& velocity );

		float GetRotation() const;
		void SetRotation( float rotation );

		float GetRotationalVelocity() const;
		void SetRotationalVelocity( float rotational_velocity );

  private:
    sf::Uint16 m_id;
    sf::Uint16 m_type;
    sf::String m_name;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity;

    float m_rotation;
    float m_rotational_velocity;

    bool m_delete_me;
    bool m_dirty;
    bool m_fresh;
};

enum packet_server_object{
  OBJECT_UPDATE = 0,
  OBJECT_STATE,
  OBJECT_REMOVE
};

typedef std::shared_ptr<Object> ObjectPtr;
typedef std::weak_ptr<Object> ObjectWeakPtr;

#endif // OBJECT_HPP_INCLUDED
