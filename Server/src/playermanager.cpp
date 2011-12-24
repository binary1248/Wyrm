#include <SFML/Network.hpp>

#include <config.hpp>
#include <objects/objects.hpp>
#include <utility.hpp>
#include <player.hpp>
#include <game.hpp>
#include <playermanager.hpp>

PlayerManager::PlayerManager() :
	m_lastId( 0 ) {
}

PlayerManager::~PlayerManager() {
}

void PlayerManager::Tick( float /*time*/ ) {
	std::list<PlayerPtr>::iterator iter( m_players.begin() );
	std::list<PlayerPtr>::iterator end( m_players.end() );

  while( iter != end ) {
    if( (*iter)->IsDeleted() ) {
    	PlayerPtr player( *iter );
      // Reap deleted players
      iter = m_players.erase( iter );

      LogConsole( "Removed player with id " + string_cast( player->GetId() ) );

      continue;
    }
    // Update connected players
    (*iter)->Update();
    ++iter;
  }
}

const PlayerPtr PlayerManager::CreatePlayer() {
  ObjectPtr agent = Game::GetGame()->GetObjectManager()->CreateObject( ObjectType::SHIP );

  agent->SetResourceId( 5 ); // Ship Mesh
  agent->SetSize( sf::Vector2f( 20.0f, 20.0f ) );

  if( !agent ) {
    return PlayerPtr();
  }

  agent->SetPosition( sf::Vector2f( 100.0f, 100.0f ) );

  PlayerPtr player = std::make_shared<Player>( agent );
  m_players.push_back( player );

  agent->SetName( player->GetName() );

  LogConsole( "Created new player: " + player->GetName() + "(id: " + string_cast( player->GetId() ) +
							") (agent id: " + string_cast( agent->GetId() ) + ")" );

  Game::GetGame()->GetObjectManager()->GetSystemById( 1 )->AddPlayer( player );

  return player;
}

sf::Uint16 PlayerManager::NewID() {
	return ++m_lastId;
}
