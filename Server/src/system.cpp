#include <SFML/Network.hpp>

#include <networkmanager.hpp>
#include <utility.hpp>
#include <system.hpp>

System::System( sf::Uint16 id, std::string name ) :
	m_id( id ),
	m_name( name ) {
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

  std::list<ObjectWeakPtr>::const_iterator object_iter( m_objects.begin() );
	std::list<ObjectWeakPtr>::const_iterator object_end( m_objects.end() );

	for( ; object_iter != object_end; ++object_iter ) {
		ObjectPtr shared_object( object_iter->lock() );

		if( shared_object && !( shared_object->IsDeleted() ) ) {
      PacketPtr packet = std::make_shared<sf::Packet>();
      shared_object->FillFullPacket( packet );
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
			char state = CLEAN;

			if( shared_object->IsDeleted() ) {
				state = DELETED;
			} else if( shared_object->IsFresh() ) {
				state = DIRTY_FULL;
				shared_object->ClearFresh();
				shared_object->ClearDirty();
			} else if( shared_object->IsDirty() ) {
				state = DIRTY_PARTIAL;
				shared_object->ClearDirty();
			}

			std::list<PlayerWeakPtr>::iterator player_iter( m_players.begin() );
			std::list<PlayerWeakPtr>::iterator player_end( m_players.end() );

			while( player_iter != player_end ) {
				PlayerPtr shared_player( player_iter->lock() );
				if( shared_player ) {
					PacketPtr packet = std::make_shared<sf::Packet>();

					switch( state ) {
						case DIRTY_PARTIAL:
							shared_object->FillPartialPacket( packet );
							shared_player->SendPacket( packet );
							break;
						case DIRTY_FULL:
							shared_object->FillFullPacket( packet );
							shared_player->SendPacket( packet );
							break;
						case DELETED:
							(*packet) << static_cast<sf::Uint16>( SERVER_OBJECT ) << shared_object->GetId() << static_cast<sf::Uint16>( OBJECT_REMOVE );
							shared_player->SendPacket( packet );
							break;
						case CLEAN:
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

			if( state == DELETED ) {
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
