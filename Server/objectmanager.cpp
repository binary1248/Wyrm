#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "player.h"
#include "objects/objects.h"
#include "game.h"
#include "objectmanager.h"

ObjectManager::ObjectManager() {
  lastId = 0;
  LastFullUpdate.Reset();
}

ObjectManager::~ObjectManager() {
  ClearObjects();
  std::cout << "Object manager cleanup done." << std::endl;
}

void ObjectManager::AddObject(Object* o) {
  objects.push_back(o);
}

void ObjectManager::RemoveObject(Object* o) {
  for( std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++ ) {
    if((*i) && (*i)->id == o->GetId()) {
      sf::Packet packet;
      packet << (sf::Uint16)OBJECT        << (sf::Uint16)0xFFFF
             << (sf::Uint16)REMOVE_OBJECT << (*i)->id;
      Game::getGame()->GetPlayerManager()->Broadcast(packet);
      delete (*i);
      objects.erase(i);
      break;
    }
  }
}

void ObjectManager::RemoveObjectById(sf::Uint16 id) {
  for( std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++ ) {
    if((*i) && (*i)->id == id) {
      sf::Packet packet;
      packet << (sf::Uint16)OBJECT        << (sf::Uint16)0xFFFF
             << (sf::Uint16)REMOVE_OBJECT << (*i)->id;
      Game::getGame()->GetPlayerManager()->Broadcast(packet);
      delete (*i);
      objects.erase(i);
      break;
    }
  }
}

Object* ObjectManager::GetObjectById(sf::Uint16 id) {
  for( size_t i = 0; i < objects.size(); i++ ) {
    if(objects[i]->id == id) {
      return objects[i];
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
    Game::getGame()->GetPlayerManager()->Broadcast(packet);
  }

  std::cout << "Created object of type " << type << std::endl;

  return object;
}

void ObjectManager::ClearObjects() {
  while( objects.size() ) {
    if( objects.back() ) {
      delete objects.back();
      objects.back() = 0;
      objects.pop_back();
    }
  }
}

void ObjectManager::SendPacketToObjectById(sf::Uint16 id, sf::Packet p) {
  for( size_t i = 0; i < objects.size(); i++ ) {
    if(objects[i]->id == id) {
      objects[i]->HandlePacket(p);
      return;
    }
  }

  std::cout << "Unhandled packet, id not found." << std::endl;
}

void ObjectManager::Tick(float time) {
  for( size_t i = 0; i < objects.size(); i++ ) {
    objects[i]->Update(time);
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
  for( size_t i = 0; i < objects.size(); i++ ) {
    sf::Packet packet;
    packet << (sf::Uint16)OBJECT << objects[i]->id << (sf::Uint16)POSITION_UPDATE
           << objects[i]->position.x << objects[i]->position.y << objects[i]->rotation;
    Game::getGame()->GetPlayerManager()->Broadcast(packet);
  }

  LastFullUpdate.Reset();
}

void ObjectManager::SendPartialUpdate() {
  for( size_t i = 0; i < objects.size(); i++ ) {
    sf::Packet packet;
    packet << (sf::Uint16)OBJECT << objects[i]->id << (sf::Uint16)VELOCITY_UPDATE
           << objects[i]->velocity.x << objects[i]->velocity.y << objects[i]->rotational_velocity;
    switch(objects[i]->type) {
      case SHIP:
        packet << ((Ship*)objects[i])->thrust;
        break;
      default:
        std::cout << "Couldn't determine what extra attributes to send." << std::endl;
        break;
    }
    Game::getGame()->GetPlayerManager()->Broadcast(packet);
  }
}

void ObjectManager::SendStateToPlayerById(sf::Uint16 id) {
  for( size_t i = 0; i < objects.size(); i++ ) {
    sf::Packet packet;
    packet << (sf::Uint16)OBJECT     << (sf::Uint16)0xFFFF
           << (sf::Uint16)NEW_OBJECT << objects[i]->type
           << objects[i]->id         << objects[i]->name
           << objects[i]->position.x << objects[i]->position.y
           << objects[i]->velocity.x << objects[i]->velocity.y
           << objects[i]->rotation   << objects[i]->rotational_velocity;
    Game::getGame()->GetPlayerManager()->SendToPlayerById(id, packet);
  }
}
