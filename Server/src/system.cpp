#include <iostream>

#include <SFML/Network.hpp>

#include <networkmanager.hpp>
#include <utility.hpp>
#include <system.hpp>

System::System( sf::Uint16 id, std::string name, sf::Uint32 background_resource_id ) :
	m_id( id ),
	m_name( name ),
	m_background_resource_id( background_resource_id ) {
}

System::~System() {
}

void System::AddPlayer( PlayerPtr player ) {
	std::list<PlayerWeakPtr>::const_iterator player_iter( m_players.begin() );
	std::list<PlayerWeakPtr>::const_iterator player_end( m_players.end() );

  for( ; player_iter != player_end; ++player_iter ) {
		if( player_iter->lock() == player ) {
			return;
		}
  }

  m_players.push_back( PlayerWeakPtr( player ) );

  player->LoadResource( m_background_resource_id );

  PacketPtr system_packet = std::make_shared<sf::Packet>();
  (*system_packet) << static_cast<sf::Uint16>( ServerToClient::SERVER_SYSTEM )
                   << GetId() << GetName() << m_background_resource_id;
  player->SendPacket( system_packet );

  std::list<ObjectWeakPtr>::const_iterator object_iter( m_objects.begin() );
	std::list<ObjectWeakPtr>::const_iterator object_end( m_objects.end() );

	for( ; object_iter != object_end; ++object_iter ) {
		ObjectPtr shared_object( object_iter->lock() );

		if( shared_object && !( shared_object->IsDeleted() ) ) {
      PacketPtr packet = std::make_shared<sf::Packet>();
      sf::Uint32 resource_id = shared_object->GetResourceId();
      shared_object->FillFullPacket( packet );
      player->LoadResource( resource_id );
      player->SendPacket( packet );
    }
	}
}

void System::AddObject( ObjectPtr object ) {
	std::list<ObjectWeakPtr>::const_iterator iter( m_objects.begin() );
	std::list<ObjectWeakPtr>::const_iterator end( m_objects.end() );

  for( ; iter != end; ++iter ) {
  	if( iter->lock() == object ) {
      return;
    }
  }

  m_objects.push_back( ObjectWeakPtr( object ) );
}

void System::Tick( float /*time*/ ) {
	std::list<ObjectWeakPtr>::iterator object_iter( m_objects.begin() );
	std::list<ObjectWeakPtr>::iterator object_end( m_objects.end() );

  while( object_iter != object_end ) {
  	ObjectPtr shared_object( object_iter->lock() );
  	if( shared_object ) {
			char state = ObjectState::CLEAN;

			if( shared_object->IsDeleted() ) {
				state = ObjectState::DELETED;
			} else if( shared_object->IsFresh() ) {
				state = ObjectState::DIRTY_FULL;
				shared_object->ClearFresh();
				shared_object->ClearDirty();
			} else if( shared_object->IsDirty() ) {
				state = ObjectState::DIRTY_PARTIAL;
				shared_object->ClearDirty();
			}

			std::list<PlayerWeakPtr>::iterator player_iter( m_players.begin() );
			std::list<PlayerWeakPtr>::iterator player_end( m_players.end() );

			while( player_iter != player_end ) {
				PlayerPtr shared_player( player_iter->lock() );
				if( shared_player ) {
					PacketPtr packet = std::make_shared<sf::Packet>();

					switch( state ) {
						case ObjectState::DIRTY_PARTIAL:
							shared_object->FillPartialPacket( packet );
							shared_player->SendPacket( packet );
							break;
						case ObjectState::DIRTY_FULL: {
							sf::Uint32 resource_id = shared_object->GetResourceId();
							shared_object->FillFullPacket( packet );
							shared_player->LoadResource( resource_id );
							shared_player->SendPacket( packet );
							break;
						}
						case ObjectState::DELETED:
							(*packet) << static_cast<sf::Uint16>( ServerToClient::SERVER_OBJECT ) << shared_object->GetId() << static_cast<sf::Uint16>( ServerToClientObject::OBJECT_REMOVE );
							shared_player->SendPacket( packet );
							break;
						case ObjectState::CLEAN:
							break;
						default:
							Die( "Unrecognized object state." );
					}
				} else {
					player_iter = m_players.erase( player_iter );
					continue;
				}

				++player_iter;
			}

			if( state == ObjectState::DELETED ) {
				object_iter = m_objects.erase( object_iter );
				continue;
			}
  	} else {
			object_iter = m_objects.erase( object_iter );
			continue;
  	}

  	++object_iter;
	}
}

sf::Uint16 System::GetId() const {
	return m_id;
}
std::string System::GetName() const {
	return m_name;
}
