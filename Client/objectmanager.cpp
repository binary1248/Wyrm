#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "network.h"
#include "player.h"
#include "game.h"
#include "objects/objects.h"
#include "objectmanager.h"

std::map< sf::Uint16, boost::function<Object* (sf::Uint16, sf::Packet&)> >* ObjectManager::factories = 0;

ObjectManager::ObjectManager() {

}

ObjectManager::~ObjectManager() {
  delete factories;
}

void ObjectManager::AddObject(Object* o) {
  for( size_t i = 0; i < objects.size(); i++ ) {
    if( objects[i] == o ) {
      return;
    }
  }

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
  p >> type;

  Object* object = 0;

  if( GetObjectById(id) ) {
    return;
  }

  if( !factories ) {
    std::cout << "No factories" << std::endl;
    return;
  }

  std::map<sf::Uint16, boost::function<Object* (sf::Uint16, sf::Packet&)> >::iterator i = factories->find(type);

  if( i == factories->end() ) {
    std::cout << "Invalid object type." << std::endl;
    return;
  } else {
    object = (i->second)(id, p);
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
    if( (*i)->IsDeleted() ) {
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
    if( objects[i]->type == SHIP && ((Ship*)objects[i])->IsPlayer() ) {
      agent = objects[i];
      continue;
    }
    objects[i]->Draw(w);
  }

  if( agent ) {
    agent->Draw(w);
  }
}

void ObjectManager::AddFactory(sf::Uint16 t, boost::function<Object* (sf::Uint16, sf::Packet&)> factory) {
  if( !factories ) {
    factories = new std::map< sf::Uint16, boost::function<Object* (sf::Uint16, sf::Packet&)> >;
  }
  ObjectManager::factories->insert( std::pair<sf::Uint16, boost::function<Object* (sf::Uint16, sf::Packet&)> >(t, factory) );
  std::cout << "Registered object factory type " << t << std::endl;
}
