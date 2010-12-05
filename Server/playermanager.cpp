#include <iostream>
#include <SFML/Network.hpp>

#include "objects/objects.h"
#include "objectmanager.h"
#include "player.h"
#include "network.h"
#include "playermanager.h"

sf::Mutex mutex;

PlayerManager playermanager;

PlayerManager::PlayerManager() {
  lastId = 0;
}

PlayerManager::~PlayerManager() {
  ClearPlayers();
  std::cout << "Player manager cleanup done." << std::endl;
}

sf::Uint16 PlayerManager::CreatePlayer(sf::String name, sf::TcpSocket* sock) {

  Player* player = new Player(lastId++, name, sock);

  mutex.Lock();
  players.push_back(player);
  mutex.Unlock();

  Object* o = objectmanager.CreateObject(SHIP);

  player->SetShip(o->id);

  std::cout << "Created new player with id " << player->id << " and ship id " << o->id << std::endl;

  sf::Packet packet;
  packet << (sf::Uint16)OBJECT     << (sf::Uint16)0xFFFF
         << (sf::Uint16)SET_ID     << o->id;
  player->SendPacket(packet);

  objectmanager.SendStateToPlayerById(player->id);

  return player->id;
}

void PlayerManager::RemovePlayer(sf::Uint16 id) {

  for( iter = players.begin(); iter != players.end(); iter++) {
    if( (*iter) && (*iter)->id == id) {
      sf::Uint16 shipId = (*iter)->shipId;
      mutex.Lock();
      delete (*iter);
      iter = players.erase(iter);
      mutex.Unlock();
      objectmanager.RemoveObjectById( shipId );
      std::cout << "Removed player with id " << (*iter)->id << " and ship id " << (*iter)->shipId << std::endl;
      return;
    }
  }
}

void PlayerManager::ClearPlayers() {

  sf::Lock Lock(mutex);
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

  sf::Lock Lock(mutex);
  for( iter = players.begin(); iter != players.end(); iter++) {
    if( (*iter) && (*iter)->id == id ) {
      // Get controlled object id and dispatch packet.
      sf::Uint16 object_id = (*iter)->shipId;
      objectmanager.SendPacketToObjectById(object_id, p);
    }
  }
}

void PlayerManager::SendToPlayerById(sf::Uint16 id, sf::Packet p) {

  sf::Lock Lock(mutex);
  for( iter = players.begin(); iter != players.end(); iter++) {
    if((*iter) && (*iter)->id == id) {
      (*iter)->SendPacket(p);
      return;
    }
  }
}

void PlayerManager::Broadcast(sf::Packet p) {

  sf::Lock Lock(mutex);
  for( iter = players.begin(); iter != players.end(); iter++) {
    (*iter)->SendPacket(p);
  }
}
