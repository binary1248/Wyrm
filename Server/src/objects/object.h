#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "../networkmanager.h"

class Object {
  public:
    Object( sf::Uint16 type, sf::String name, sf::Vector2f position,
						sf::Vector2f velocity, float rotation, float rotational_velocity );
    virtual ~Object();

    virtual void Update( float time );

    // Network Handlers
    virtual void FillPartialPacket( PacketPtr packet );
    virtual void FillFullPacket( PacketPtr packet ) = 0;
    virtual void HandlePacket( PacketPtr packet ) = 0;

    // ID Handlers
    sf::Uint16 GetId();
    sf::Uint16 GetType();
    void SetName( sf::String name );
    sf::String GetName();

    // Dirty flag Handlers
    bool IsDirty();
    void SetDirty();
    void ClearDirty();

    bool IsFresh();
    void SetFresh();
    void ClearFresh();

    // Delete Handlers
    void Delete();
    bool IsDeleted();

    sf::Vector2f GetPosition();
    void SetPosition( sf::Vector2f position );

		sf::Vector2f GetVelocity();
		void SetVelocity( sf::Vector2f velocity );

		float GetRotation();
		void SetRotation( float rotation );

		float GetRotationalVelocity();
		void SetRotationalVelocity( float rotational_velocity );

  private:
    sf::Uint16 m_id;
    sf::Uint16 m_type;
    sf::String m_name;

    bool m_dirty;
    bool m_fresh;
    bool m_delete_me;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity;

    float m_rotation;
    float m_rotational_velocity;
};

enum packet_server_object{
  OBJECT_UPDATE = 0,
  OBJECT_STATE,
  OBJECT_REMOVE
};

typedef boost::shared_ptr<Object> ObjectPtr;
typedef boost::weak_ptr<Object> ObjectWeakPtr;

#endif // OBJECT_H_INCLUDED
