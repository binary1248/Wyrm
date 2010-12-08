#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "player.h"
#include "objects/objects.h"
#include "game.h"
#include "objectmanager.h"

ObjectManager::ObjectManager(Game* g) {
  lastId = 0;
  game = g;
  LastFullUpdate.Reset();
}

ObjectManager::~ObjectManager() {
  ClearObjects();
  std::cout << "Object manager cleanup done." << std::endl;
}

void ObjectManager::AddObject(Object* o) {
  objects.push_back(o);
}

void ObjectManager::RemoveObjectById(sf::Uint16 id) {
  for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    if((*i) && (*i)->id == id) {
      sf::Packet packet;
      packet << (sf::Uint16)OBJECT        << (sf::Uint16)0xFFFF
             << (sf::Uint16)REMOVE_OBJECT << (*i)->id;
      game->GetPlayerManager()->Broadcast(packet);
      delete (*i);
      objects.erase(i);
      break;
    }
  }
}

Object* ObjectManager::GetObjectById(sf::Uint16 id) {
  for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    if((*i)->id == id) {
      return (*i);
    }
  }
  std::cout << "Couldn't find object with id " << id << std::endl;
  return 0;
}

Object* ObjectManager::CreateObject(sf::Uint16 type) {

  Object* object = 0;

  switch(type) {
    case SHIP:
      object = new Ship(lastId++, "", sf::Vector2f(0,0), sf::Vector2f(0,0), 0, 0);
      break;
    default:
      std::cout << "Invalid object type." << std::endl;
      break;
  }
  if(object) {
    AddObject(object);
    sf::Packet packet;
    packet << (sf::Uint16)OBJECT     << (sf::Uint16)0xFFFF
           << (sf::Uint16)NEW_OBJECT << object->type
           << object->id             << object->name
           << object->position.x     << object->position.y
           << object->velocity.x     << object->velocity.y
           << object->rotation       << object->rotational_velocity;
    game->GetPlayerManager()->Broadcast(packet);
  }

  std::cout << "Created object of type " << type << std::endl;

  return object;
}

void ObjectManager::ClearObjects() {
  for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end();) {
    if(*i) {
      delete (*i);
      (*i) = 0;
      objects.erase(i);
    }
  }
}

void ObjectManager::SendPacketToObjectById(sf::Uint16 id, sf::Packet p) {
  for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    if((*i)->id == id) {
      (*i)->HandlePacket(p);
      return;
    }
  }

  std::cout << "Unhandled packet, id not found." << std::endl;
}

void ObjectManager::Tick(float time) {
  for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    (*i)->Update(time);
  }

  SendUpdate();
}

void ObjectManager::SendUpdate() {
  if( LastFullUpdate.GetElapsedTime() < 2.0f ) {
    SendPartialUpdate();
  } else {
    SendFullUpdate();
  }
}

void ObjectManager::SendFullUpdate() {
  for( std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    sf::Packet packet;
    packet << (sf::Uint16)OBJECT << (*i)->id << (sf::Uint16)POSITION_UPDATE
           << (*i)->position.x << (*i)->position.y << (*i)->rotation;
    game->GetPlayerManager()->Broadcast(packet);
  }

  LastFullUpdate.Reset();
}

void ObjectManager::SendPartialUpdate() {
  for( std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    sf::Packet packet;
    packet << (sf::Uint16)OBJECT << (*i)->id << (sf::Uint16)VELOCITY_UPDATE
           << (*i)->velocity.x << (*i)->velocity.y << (*i)->rotational_velocity;
    switch((*i)->type) {
      case SHIP:
        packet << ((Ship*)(*i))->thrust;
        break;
      default:
        std::cout << "Couldn't determine what extra attributes to send." << std::endl;
        break;
    }
    game->GetPlayerManager()->Broadcast(packet);
  }
}

void ObjectManager::SendStateToPlayerById(sf::Uint16 id) {
  for( std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    sf::Packet packet;
    packet << (sf::Uint16)OBJECT     << (sf::Uint16)0xFFFF
           << (sf::Uint16)NEW_OBJECT << (*i)->type
           << (*i)->id               << (*i)->name
           << (*i)->position.x       << (*i)->position.y
           << (*i)->velocity.x       << (*i)->velocity.y
           << (*i)->rotation         << (*i)->rotational_velocity;
    game->GetPlayerManager()->SendToPlayerById(id, packet);
  }
}
