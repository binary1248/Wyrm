#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include "objects/objects.h"
#include "utility.h"
#include "player.h"
#include "game.h"
#include "playermanager.h"

PlayerManager::PlayerManager() {
  lastId = 0;
}

PlayerManager::~PlayerManager() {
  ClearPlayers();
  LogConsole("Player manager cleanup done.");
}

void PlayerManager::Tick(float time) {
  for( std::vector<Player*>::iterator iter = players.begin(); iter != players.end(); ) {
    if( (*iter)->IsDeleted() ) {
      // Reap deleted players
      RemovePlayer(*iter);
      continue;
    }
    // Update connected players
    (*iter)->Update();
    iter++;
  }
}

Player* PlayerManager::CreatePlayer() {
  Object* o = Game::GetGame()->GetObjectManager()->CreateObject(SHIP);

  if( !o ) {
    return 0;
  }

  Player* player = new Player(o);
  players.push_back(player);

  o->SetName( player->GetName() );

  std::stringstream ss;
  ss << "Created new player with id " << player->GetId() << " and ship id " << o->GetId();
  LogConsole(ss.str());

  return player;
}

void PlayerManager::RemovePlayer(Player* p) {
  for( std::vector<Player*>::iterator iter = players.begin(); iter != players.end(); iter++) {
    if( (*iter) == p ) {
      std::stringstream ss;
      ss << "Removed player with id " << p->GetId();
      LogConsole(ss.str());

      delete p;
      p = 0;
      iter = players.erase(iter);

      return;
    }
  }
}

void PlayerManager::ClearPlayers() {
  while(!players.empty()) {
    if(players.back()) {
      delete players.back();
      players.back() = 0;
      players.pop_back();
    }
  }
}

void PlayerManager::SendToPlayerById(sf::Uint16 id, sf::Packet p) {
  for( std::size_t i = 0; i < players.size(); i++ ) {
    if( players[i] && players[i]->GetId() == id) {
      players[i]->SendPacket(p);
      return;
    }
  }
}

void PlayerManager::BroadcastNewObject(Object* o) {
  for( std::size_t i = 0; i < players.size(); i++ ) {
    players[i]->AddObjectToView(o);
  }
}

void PlayerManager::Broadcast(sf::Packet p) {
  for( std::size_t i = 0; i < players.size(); i++ ) {
    players[i]->SendPacket(p);
  }
}
