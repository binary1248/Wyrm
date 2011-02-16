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
  std::cout << "Player manager cleanup done." << std::endl;
}

void PlayerManager::Tick(float time) {
  for( std::vector<Player*>::iterator iter = players.begin(); iter != players.end(); ) {
    if( !(*iter)->Connected() ) {
      // Reap disconnected players
      std::cout << "Reaping disconnected player" << std::endl;
      Object* agent = (*iter)->GetAgent();
      delete (*iter);
      iter = players.erase(iter);
      Game::getGame()->GetObjectManager()->RemoveObjectById( agent->GetId() );
      std::cout << "Removed player with id " << (*iter)->GetId() << " and agent id " << (*iter)->GetAgent()->GetId() << std::endl;
      continue;
    }
    (*iter)->Update();
    iter++;
  }
}

sf::Uint16 PlayerManager::CreatePlayer(sf::TcpSocket* sock) {
  Player* player = new Player(lastId++, sock);

  players.push_back(player);

  Object* o = Game::getGame()->GetObjectManager()->CreateObject(SHIP);

  player->SetAgent(o);

  std::cout << "Created new player with id " << player->GetId() << " and ship id " << o->id << std::endl;

  sf::Packet packet;
  packet << (sf::Uint16)OBJECT     << (sf::Uint16)0xFFFF
         << (sf::Uint16)SET_ID     << o->id;
  player->SendPacket(packet);

  Game::getGame()->GetObjectManager()->SendStateToPlayerById(player->GetId());

  return player->GetId();
}

void PlayerManager::RemovePlayer(sf::Uint16 id) {
  for( std::vector<Player*>::iterator iter = players.begin(); iter != players.end(); iter++) {
    if( (*iter) && (*iter)->GetId() == id) {
      Object* agent = (*iter)->GetAgent();
      delete (*iter);
      iter = players.erase(iter);
      Game::getGame()->GetObjectManager()->RemoveObjectById( agent->GetId() );
      std::cout << "Removed player with id " << (*iter)->GetId() << " and agent id " << (*iter)->GetAgent()->GetId() << std::endl;
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

void PlayerManager::Broadcast(sf::Packet p) {
  for( std::size_t i = 0; i < players.size(); i++ ) {
    players[i]->SendPacket(p);
  }
}
