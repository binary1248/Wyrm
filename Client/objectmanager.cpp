#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "network.h"
#include "player.h"
#include "objects/objects.h"
#include "objectmanager.h"

ObjectManager objectmanager;

ObjectManager::ObjectManager() {

}

ObjectManager::~ObjectManager() {

}

void ObjectManager::AddObject(Object* o) {
  objects.push_back(o);
}

void ObjectManager::RemoveObjectById(sf::Uint16 id) {
  for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    if((*i)->id == id) {
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
  return 0;
}

void ObjectManager::CreateObject(sf::Packet p) {
  sf::Uint16 type;
  sf::Uint16 id;
  sf::String name;
  sf::Vector2f pos;
  sf::Vector2f vel;
  float rot;
  float rot_vel;

  p >> type >> id >> name >> pos.x >> pos.y >> vel.x >> vel.y >> rot >> rot_vel;

  Object* o = GetObjectById(id);

  if( o ) {
    o->name = name;
    o->position = pos;
    o->velocity = vel;
    o->rotation = rot;
    o->rotational_velocity = rot_vel;
    return;
  }

  Object* object = 0;

  switch(type) {
    case SHIP:
      object = new Ship(id, name, pos, vel, rot, rot_vel);
      break;
    default:
      std::cout << "Invalid object type." << std::endl;
      break;
  }
  if(object) {
    AddObject(object);
  }
}

void ObjectManager::DispatchPacket(sf::Packet p) {
  sf::Uint16 id;
  p >> id;
  if( id == 0xFFFF ) {
    sf::Uint16 type1;
    p >> type1;
    switch(type1) {
      case NEW_OBJECT: {
        CreateObject(p);
        break;
      }
      case REMOVE_OBJECT: {
        sf::Uint16 id;
        p >> id;
        objectmanager.RemoveObjectById(id);
        break;
      }
      case SET_ID: {
        sf::Uint16 pid_;
        p >> pid_;
        if(!player) {
          player = new Player(0,"");
        }
        player->SetShip(pid_);
        break;
      }
      default:
        break;
    }
  }
  else {
    for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
      if((*i)->id == id) {
        (*i)->HandlePacket(p);
        break;
      }
    }
  }
}

void ObjectManager::Tick(float time) {
  for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    (*i)->Update(time);
  }
}

void ObjectManager::DrawAll(sf::RenderWindow& w) {
  for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    (*i)->Draw(w);
  }
}
