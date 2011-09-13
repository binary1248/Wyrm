#include <iostream>

#include <boost/make_shared.hpp>

#include <SFML/Network.hpp>

#include "objects/objects.h"
#include "utility.h"
#include "player.h"
#include "game.h"
#include "playermanager.h"

PlayerManager::PlayerManager() :
	m_lastId( 0 ) {
}

PlayerManager::~PlayerManager() {
}

void PlayerManager::Tick(float time) {
  for( std::list<PlayerPtr>::iterator iter = m_players.begin(); iter != m_players.end(); ) {
    if( (*iter)->IsDeleted() ) {
    	PlayerPtr player( *iter );
      // Reap deleted players
      iter = m_players.erase( iter );

      LogConsole( "Removed player with id " + STRING_CAST( player->GetId() ) );

      continue;
    }
    // Update connected players
    (*iter)->Update();
    iter++;
  }
}

PlayerPtr PlayerManager::CreatePlayer() {
  ObjectPtr agent = Game::GetGame()->GetObjectManager()->CreateObject(SHIP);

  if( !agent ) {
    return PlayerPtr();
  }

  agent->SetPosition(sf::Vector2f(100,100));

  PlayerPtr player = boost::make_shared<Player>( agent );
  m_players.push_back( player );

  agent->SetName( player->GetName() );

  LogConsole( "Created new player: " + player->GetName() + "(id: " + STRING_CAST( player->GetId() ) +
							") (agent id: " + STRING_CAST( agent->GetId() ) + ")" );

  Game::GetGame()->GetObjectManager()->GetSystemById( 0 )->AddPlayer( player );

  return player;
}

sf::Uint16 PlayerManager::NewID() {
	return m_lastId++;
}
