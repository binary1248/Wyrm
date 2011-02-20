#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "network.h"
#include "player.h"
#include "game.h"
#include "objects/objects.h"
#include "objectmanager.h"

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

void ObjectManager::CreateObject(sf::Packet p, sf::Uint16 id) {
  sf::Uint16 type;
  sf::String name;
  sf::Vector2f pos;
  sf::Vector2f vel;
  float rot;
  float rot_vel;

  p >> type >> name >> pos.x >> pos.y >> vel.x >> vel.y >> rot >> rot_vel;

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
      p >> ((Ship*)object)->thrust;
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

  for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++) {
    if((*i)->id == id) {
      (*i)->HandlePacket(p);
      return;
    }
  }

  sf::Uint16 type1;
  p >> type1;

  if( type1 != STATE ) {
    std::cout << "Can't update non-existant object: " << id << std::endl;
    return;
  }

  CreateObject(p, id);
}

void ObjectManager::Tick(float time) {
  for(std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); ) {
    if( (*i)->FlaggedForRemoval() ) {
      delete (*i);
      i = objects.erase(i);
      continue;
    }
    (*i)->Update(time);
    i++;
  }
}

void ObjectManager::DrawAll(sf::RenderWindow& w) {
  Object* agent = 0;
  for(size_t i = 0; i < objects.size(); i++) {
    if( objects[i]->type == SHIP && ((Ship*)objects[i])->isPlayer ) {
      agent = objects[i];
      continue;
    }
    objects[i]->Draw(w);
  }

  if( agent ) {
    agent->Draw(w);
  }
}
