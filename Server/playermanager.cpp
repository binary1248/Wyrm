#include <iostream>
#include <SFML/Network.hpp>

#include "objects/objects.h"
#include "player.h"
#include "game.h"
#include "playermanager.h"

PlayerManager::PlayerManager(Game* g) {
  lastId = 0;
  game = g;
}

PlayerManager::~PlayerManager() {
  ClearPlayers();
  std::cout << "Player manager cleanup done." << std::endl;
}

void PlayerManager::Tick(float time) {

}

sf::Uint16 PlayerManager::CreatePlayer(sf::String name, sf::TcpSocket* sock) {
  Player* player = new Player(lastId++, name, sock);

  players.push_back(player);

  Object* o = game->GetObjectManager()->CreateObject(SHIP);

  player->SetShip(o->id);

  std::cout << "Created new player with id " << player->id << " and ship id " << o->id << std::endl;

  sf::Packet packet;
  packet << (sf::Uint16)OBJECT     << (sf::Uint16)0xFFFF
         << (sf::Uint16)SET_ID     << o->id;
  player->SendPacket(packet);

  game->GetObjectManager()->SendStateToPlayerById(player->id);

  return player->id;
}

void PlayerManager::RemovePlayer(sf::Uint16 id) {
  for( iter = players.begin(); iter != players.end(); iter++) {
    if( (*iter) && (*iter)->id == id) {
      sf::Uint16 shipId = (*iter)->shipId;
      delete (*iter);
      iter = players.erase(iter);
      game->GetObjectManager()->RemoveObjectById( shipId );
      std::cout << "Removed player with id " << (*iter)->id << " and ship id " << (*iter)->shipId << std::endl;
      return;
    }
  }
}

void PlayerManager::ClearPlayers() {
  for(iter = players.begin(); iter != players.end(); ) {
    if(*iter) {
      (*iter)->Disconnect();
      delete (*iter);
      (*iter) = 0;
      iter = players.erase(iter);
    }
  }
}

void PlayerManager::DispatchPacket(sf::Packet p, sf::Uint16 id) {
  for( iter = players.begin(); iter != players.end(); iter++) {
    if( (*iter) && (*iter)->id == id ) {
      // Get controlled object id and dispatch packet.
      sf::Uint16 object_id = (*iter)->shipId;
      game->GetObjectManager()->SendPacketToObjectById(object_id, p);
    }
  }
}

void PlayerManager::SendToPlayerById(sf::Uint16 id, sf::Packet p) {
  for( iter = players.begin(); iter != players.end(); iter++) {
    if((*iter) && (*iter)->id == id) {
      (*iter)->SendPacket(p);
      return;
    }
  }
}

void PlayerManager::Broadcast(sf::Packet p) {
  for( iter = players.begin(); iter != players.end(); iter++) {
    (*iter)->SendPacket(p);
  }
}
