#include <config.hpp>

#include <SFML/System.hpp>

#include <player.hpp>
#include <objects/objects.hpp>
#include <game.hpp>
#include <utility.hpp>
#include <objectmanager.hpp>

FactoryMap* ObjectManager::m_factories = 0;

ObjectManager::ObjectManager() :
	m_lastSystemId( 0 ),
	m_lastObjectId( 0 ),
	m_objects_loaded( false ),
	m_factories_auto_delete( m_factories ) {
}

ObjectManager::~ObjectManager() {
}

const ObjectPtr ObjectManager::GetObjectById( sf::Uint16 id ) const {
	std::list<ObjectPtr>::const_iterator iter( m_objects.begin() );
	std::list<ObjectPtr>::const_iterator end( m_objects.end() );

	for( ; iter != end; ++iter ) {
		if( (*iter)->GetId() == id ) {
      return (*iter);
    }
	}

  LogConsole( "Couldn't find object with id " + string_cast( id ) );

  return ObjectPtr();
}

const ObjectPtr ObjectManager::CreateObject( sf::Uint16 type ) {
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
		GetSystemById( 1 )->AddObject( object );
    return object;
  }
}

const SystemPtr ObjectManager::GetSystemById( sf::Uint16 id ) const {
	std::list<SystemPtr>::const_iterator iter( m_systems.begin() );
	std::list<SystemPtr>::const_iterator end( m_systems.end() );

	for( ; iter != end; ++iter ) {
		if( (*iter)->GetId() == id ) {
      return (*iter);
    }
	}

  LogConsole( "Couldn't find system with id " + string_cast( id ) );

  return SystemPtr();
}

void ObjectManager::CreateSystem( std::string name, sf::Uint32 background_resource_id ) {
  SystemPtr system = std::make_shared<System>( ++m_lastSystemId, name, background_resource_id );

  LogConsole( "Created new system: " + system->GetName() + " (id: " + string_cast( system->GetId() ) + ")" );

  m_systems.push_back( system );
}

void ObjectManager::Tick( float time ) {
  if( !m_objects_loaded ) {
    LoadObjects();
  }

  std::list<SystemPtr>::const_iterator system_iter( m_systems.begin() );
	std::list<SystemPtr>::const_iterator system_end( m_systems.end() );

  for( ; system_iter != system_end; ++system_iter ) {
		(*system_iter)->Tick( time );
  }

  std::list<ObjectPtr>::iterator object_iter( m_objects.begin() );
  std::list<ObjectPtr>::iterator object_end( m_objects.end() );

  while( object_iter != object_end ) {
    if( (*object_iter)->IsDeleted() ) {
      object_iter = m_objects.erase( object_iter );
      continue;
    }

    (*object_iter)->Update( time );

    ++object_iter;
  }
}

sf::Uint16 ObjectManager::NewID() {
	return ++m_lastObjectId;
}

void ObjectManager::AddFactory( sf::Uint16 type, ObjectFactory factory ) {
	if( !m_factories ) {
		m_factories = new FactoryMap;
	}

  m_factories->insert( std::pair<sf::Uint16, ObjectFactory>( type, factory ) );

  LogStartup( "Registered object factory type " + string_cast( type ) );
}

void ObjectManager::LoadObjects() {
	LogConsole( "Loading objects..." );

  CreateSystem( "System 1", 1 );

  PlanetPtr p( std::static_pointer_cast<Planet, Object>( CreateObject( ObjectType::PLANET ) ) );
  p->SetSize( sf::Vector2f( 80.0f, 80.0f ) );
  p->SetOrbit( 2, 400 );
  p->SetRotationalVelocity( 2 );
  p->SetAnchor( 0, 0 );
  p->SetName( "Planet 1" );
  p->SetResourceId( 2 );

  StarPtr s( std::static_pointer_cast<Star, Object>( CreateObject( ObjectType::STAR ) ) );
  s->SetOrbit( 0, 0 );
  s->SetRotationalVelocity( 0 );
  s->SetAnchor( 0, 0 );
  s->SetName( "Star 1" );
  s->SetResourceId( 4 );

  m_objects_loaded = true;

  LogConsole( "Objects loaded" );
}
