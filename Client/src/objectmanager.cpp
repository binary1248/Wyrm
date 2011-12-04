#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <config.hpp>
#include <network.hpp>
#include <player.hpp>
#include <game.hpp>
#include <objects/objects.hpp>
#include <objectmanager.hpp>

FactoryMap* ObjectManager::m_factories = 0;

ObjectManager::ObjectManager() :
	m_factories_auto_delete( m_factories ) {
}

ObjectManager::~ObjectManager() {
}

ObjectPtr ObjectManager::GetObjectById( sf::Uint16 id ) {
	std::list<ObjectPtr>::iterator iter( m_objects.begin() );
	std::list<ObjectPtr>::iterator end( m_objects.end() );

  for( ; iter != end; ++iter ) {
    if( ( *iter )->GetID() == id ) {
      return ( *iter );
    }
  }

  return ObjectPtr();
}

void ObjectManager::CreateObject( sf::Packet packet, sf::Uint16 id ) {
  sf::Uint16 type;
  packet >> type;

  ObjectPtr object;

  if( GetObjectById( id ) ) {
    return;
  }

  if( !m_factories ) {
    std::cout << "No factories\n";
    return;
  }

  std::map<sf::Uint16, ObjectFactory>::iterator i = m_factories->find( type );

  if( i == m_factories->end() ) {
    std::cout << "Invalid object type.\n";
    return;
  } else {
    object = ( i->second )( id, packet );
  }

  m_objects.push_back( object );
}

void ObjectManager::DispatchPacket( sf::Packet packet ) {
  sf::Uint16 id;
  packet >> id;

  std::list<ObjectPtr>::iterator iter( m_objects.begin() );
	std::list<ObjectPtr>::iterator end( m_objects.end() );

  for( ; iter != end; ++iter ) {
    if( ( *iter )->GetID() == id ) {
      ( *iter )->HandlePacket( packet );
      return;
    }
  }

  sf::Uint16 type1;
  packet >> type1;

  if( type1 != OBJECT_STATE ) {
    std::cout << "Can't update non-existant object: " << id << "\n";
    return;
  }

  CreateObject( packet, id );
}

void ObjectManager::Tick( float time ) {
	std::list<ObjectPtr>::iterator iter( m_objects.begin() );
	std::list<ObjectPtr>::iterator end( m_objects.end() );

  while( iter != end ) {
    if( ( *iter )->IsDeleted() ) {
      iter = m_objects.erase( iter );
      continue;
    }

    ( *iter )->Update( time );
    ++iter;
  }
}

void ObjectManager::DrawAll( sf::RenderWindow& target ) {
  ObjectPtr agent;

  std::list<ObjectPtr>::iterator iter( m_objects.begin() );
	std::list<ObjectPtr>::iterator end( m_objects.end() );

  for( ; iter != end; ++iter ) {
    if( ( *iter )->GetType() == SHIP && std::static_pointer_cast<Ship>( ( *iter ) )->IsPlayer() ) {
      agent = ( *iter );
      continue;
    }
    ( *iter )->Draw( target );
  }

	// Make sure player agent is drawn on top of everything else
  if( agent ) {
    agent->Draw( target );
  }
}

void ObjectManager::AddFactory( sf::Uint16 type, ObjectFactory factory ) {
  if( !m_factories ) {
    m_factories = new FactoryMap;
  }

  ObjectManager::m_factories->insert( std::pair<sf::Uint16, ObjectFactory>( type, factory ) );

  std::cout << "Registered object factory type " << type << "\n";
}
