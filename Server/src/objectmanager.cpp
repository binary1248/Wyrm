#include <iostream>

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include <SFML/System.hpp>

#include "player.h"
#include "objects/objects.h"
#include "game.h"
#include "utility.h"
#include "objectmanager.h"

FactoryMap* ObjectManager::m_factories = 0;

ObjectManager::ObjectManager() :
	m_lastObjectId( 0 ),
	m_lastSystemId( 0 ),
	m_objects_loaded( false ),
	m_factories_auto_delete( m_factories ) {
}

ObjectManager::~ObjectManager() {
}

ObjectPtr ObjectManager::GetObjectById( sf::Uint16 id ) const {
	BOOST_FOREACH( ObjectPtr object, m_objects ) {
		if( object->GetId() == id ) {
      return object;
    }
	}

  LogConsole( "Couldn't find object with id " + STRING_CAST( id ) );

  return ObjectPtr();
}

ObjectPtr ObjectManager::CreateObject( sf::Uint16 type ) {
  if( !m_factories ) {
    Die( "Tried to create object without factories." );
  }

  std::map<sf::Uint16, ObjectFactory>::iterator iter = m_factories->find( type );

  if( iter == m_factories->end() ) {
    LogConsole( "Invalid object type." );
    return ObjectPtr();
  } else {
    ObjectPtr object = ( iter->second )();
    m_objects.push_back( object );
		GetSystemById( 0 )->AddObject( object );
    return object;
  }
}

SystemPtr ObjectManager::GetSystemById( sf::Uint16 id ) const {
	BOOST_FOREACH( SystemPtr system, m_systems ) {
		if( system->GetId() == id ) {
      return system;
    }
	}

  LogConsole( "Couldn't find systems with id " + STRING_CAST( id ) );

  return SystemPtr();
}

void ObjectManager::CreateSystem( std::string name ) {
  SystemPtr system = boost::make_shared<System>( m_lastSystemId++, name );

  LogConsole( "Created new system: " + system->GetName() + " (id: " + STRING_CAST( system->GetId() ) + ")" );

  m_systems.push_back( system );
}

void ObjectManager::Tick( float time ) {
  if( !m_objects_loaded ) {
    LoadObjects();
  }

  BOOST_FOREACH( SystemPtr system, m_systems ) {
		system->Tick( time );
  }

  for( std::list<ObjectPtr>::iterator iter = m_objects.begin(); iter != m_objects.end(); ) {
    if( (*iter)->IsDeleted() ) {
      iter = m_objects.erase( iter );
      continue;
    }

    (*iter)->Update( time );

    iter++;
  }
}

sf::Uint16 ObjectManager::NewID() {
	return m_lastObjectId++;
}

void ObjectManager::AddFactory( sf::Uint16 type, ObjectFactory factory ) {
	if( !m_factories ) {
		m_factories = new FactoryMap;
	}

  m_factories->insert( std::pair<sf::Uint16, ObjectFactory>( type, factory ) );

  LogStartup( "Registered object factory type " + STRING_CAST( type ) );
}

void ObjectManager::LoadObjects() {
  CreateSystem( "System 1" );

  PlanetPtr p( boost::static_pointer_cast<Planet, Object>( CreateObject( PLANET ) ) );
  p->SetOrbit( 2, 400 );
  p->SetRotationalVelocity( 2 );
  p->SetAnchor( 0, 0 );
  p->SetName( "Planet 1" );

  StarPtr s( boost::static_pointer_cast<Star, Object>( CreateObject( STAR ) ) );
  s->SetOrbit( 0, 0 );
  s->SetRotationalVelocity( 0 );
  s->SetAnchor( 0, 0 );
  s->SetName( "Star 1" );

  m_objects_loaded = true;
}
