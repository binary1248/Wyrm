#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "network.h"
#include "player.h"
#include "game.h"
#include "objects/objects.h"
#include "objectmanager.h"

FactoryMap* ObjectManager::factories = 0;

ObjectManager::ObjectManager() :
	m_factories_auto_delete( factories ) {
}

ObjectManager::~ObjectManager() {
}

ObjectPtr ObjectManager::GetObjectById( sf::Uint16 id ) {
  for( std::vector<ObjectPtr>::iterator i = objects.begin(); i != objects.end(); i++ ) {
    if( (*i)->id == id ) {
      return (*i);
    }
  }

  return ObjectPtr();
}

void ObjectManager::CreateObject(sf::Packet p, sf::Uint16 id) {
  sf::Uint16 type;
  p >> type;

  ObjectPtr object;

  if( GetObjectById(id) ) {
    return;
  }

  if( !factories ) {
    std::cout << "No factories" << std::endl;
    return;
  }

  std::map<sf::Uint16, boost::function<ObjectPtr (sf::Uint16, sf::Packet&)> >::iterator i = factories->find(type);

  if( i == factories->end() ) {
    std::cout << "Invalid object type." << std::endl;
    return;
  } else {
    object = (i->second)(id, p);
  }

  objects.push_back( object );
}

void ObjectManager::DispatchPacket(sf::Packet p) {
  sf::Uint16 id;
  p >> id;

  for(std::vector<ObjectPtr>::iterator i = objects.begin(); i != objects.end(); i++) {
    if((*i)->id == id) {
      (*i)->HandlePacket(p);
      return;
    }
  }

  sf::Uint16 type1;
  p >> type1;

  if( type1 != OBJECT_STATE ) {
    std::cout << "Can't update non-existant object: " << id << std::endl;
    return;
  }

  CreateObject(p, id);
}

void ObjectManager::Tick(float time) {
  for(std::vector<ObjectPtr>::iterator i = objects.begin(); i != objects.end(); ) {
    if( (*i)->IsDeleted() ) {
      i = objects.erase(i);
      continue;
    }
    (*i)->Update(time);
    i++;
  }
}

void ObjectManager::DrawAll(sf::RenderWindow& w) {
  ObjectPtr agent;

  for(size_t i = 0; i < objects.size(); i++) {
    if( objects[i]->type == SHIP && boost::static_pointer_cast<Ship, Object>( objects[i] )->IsPlayer() ) {
      agent = objects[i];
      continue;
    }
    objects[i]->Draw(w);
  }

	// Make sure player agent is drawn on top of everything else
  if( agent ) {
    agent->Draw(w);
  }
}

void ObjectManager::AddFactory(sf::Uint16 t, boost::function<ObjectPtr (sf::Uint16, sf::Packet&)> factory) {
  if( !factories ) {
    factories = new FactoryMap;
  }
  ObjectManager::factories->insert( std::pair<sf::Uint16, ObjectFactory>(t, factory) );
  std::cout << "Registered object factory type " << t << std::endl;
}
