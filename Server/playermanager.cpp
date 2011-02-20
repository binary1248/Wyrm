#include <iostream>
#include <SFML/Network.hpp>

#include "objects/objects.h"
#include "player.h"
#include "game.h"
#include "playermanager.h"

PlayerManager::PlayerManager() {
  lastId = 0;
}

PlayerManager::~PlayerManager() {
  ClearPlayers();
  std::cout << TIME << "Player manager cleanup done." << std::endl;
}

void PlayerManager::Tick(float time) {
  for( std::vector<Player*>::iterator iter = players.begin(); iter != players.end(); ) {
    if( !(*iter)->Alive() ) {
      // Reap disconnected players
      std::cout << TIME << "Removing player with id " << (*iter)->GetId() << " and agent id " << (*iter)->GetAgent()->GetId() << std::endl;
      delete (*iter);
      iter = players.erase(iter);
      continue;
    }
    // Update connected players
    (*iter)->Update();
    iter++;
  }
}

sf::Uint16 PlayerManager::CreatePlayer(sf::TcpSocket* sock) {
  Object* o = Game::GetGame()->GetObjectManager()->CreateObject(SHIP);
  o->SetName( sock->GetRemoteAddress().ToString() );
  if( !o ) {
    sock->Disconnect();
    return 0xFFFF;
  }
  Player* player = new Player(sock, o);
  players.push_back(player);
  std::cout << TIME << "Created new player with id " << player->GetId() << " and ship id " << o->id << std::endl;
  return player->GetId();
}

void PlayerManager::RemovePlayer(sf::Uint16 id) {
  for( std::vector<Player*>::iterator iter = players.begin(); iter != players.end(); iter++) {
    if( (*iter) && (*iter)->GetId() == id) {
      Object* agent = (*iter)->GetAgent();
      delete (*iter);
      iter = players.erase(iter);
      Game::GetGame()->GetObjectManager()->RemoveObjectById( agent->GetId() );
      std::cout << TIME << "Removed player with id " << (*iter)->GetId() << " and agent id " << (*iter)->GetAgent()->GetId() << std::endl;
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
