#include <iostream>
#include <SFML/System.hpp>
#include "player.h"
#include "objects/objects.h"
#include "game.h"
#include "objectmanager.h"

std::map< sf::Uint16, boost::function<Object* ()> >* ObjectManager::factories = 0;

ObjectManager::ObjectManager() {
  lastId = 0;
  LastFullUpdate.Reset();
  objects_loaded = false;
}

ObjectManager::~ObjectManager() {
  ClearObjects();
  std::cout << TIME << "Object manager cleanup done." << std::endl;
  delete factories;
}

void ObjectManager::AddObject(Object* o) {
  objects.push_back(o);
}

void ObjectManager::RemoveObject(Object* o) {
  for( std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++ ) {
    if((*i) && (*i)->id == o->GetId()) {
      delete (*i);
      objects.erase(i);
      break;
    }
  }
}

void ObjectManager::RemoveObjectById(sf::Uint16 id) {
  for( std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); i++ ) {
    if((*i) && (*i)->id == id) {
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
  std::cout << TIME << "Couldn't find object with id " << id << std::endl;
  return 0;
}

Object* ObjectManager::CreateObject(sf::Uint16 type) {

  if( !factories ) {
    std::cout << TIME << "No factories" << std::endl;
    return 0;
  }

  std::map<sf::Uint16, boost::function<Object* ()> >::iterator i = factories->find(type);

  if( i == factories->end() ) {
    std::cout << TIME << "Invalid object type." << std::endl;
    return 0;
  } else {
    return (i->second)();
  }
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

void ObjectManager::SubscribeRelevant(Player* p) {
  for( size_t i = 0; i < objects.size(); i++ ) {
    if( p->GetAgent() != objects[i] ) {
      p->AddObjectToView(objects[i]);
    }
  }
}

void ObjectManager::Tick(float time) {
  if( !objects_loaded ) {
    LoadObjects();
  }

  for( size_t i = 0; i < objects.size(); i++ ) {
    if( objects[i]->IsFresh() ) {
      Game::GetGame()->GetPlayerManager()->BroadcastNewObject(objects[i]);
      objects[i]->ClearFresh();
    }
    objects[i]->Update(time);
  }
}

void ObjectManager::AddFactory(sf::Uint16 t, boost::function<Object* ()> factory) {
  if( !factories ) {
    factories = new std::map< sf::Uint16, boost::function<Object* ()> >;
  }
  ObjectManager::factories->insert( std::pair<sf::Uint16, boost::function<Object* ()> >(t, factory) );
  std::cout << TIME << "Registered object factory type " << t << std::endl;
}

void ObjectManager::LoadObjects() {
  Planet* p = (Planet*)(CreateObject(PLANET));
  p->velocity.x = 5; // Orbital vel
  p->velocity.y = 400; // Orbital rad
  p->rotational_velocity = 5;
  p->anchor.x = 100;
  p->anchor.y = -50;

  objects_loaded = true;
}
















