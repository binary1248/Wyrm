#include <sstream>
#include <SFML/Network.hpp>
#include "networkmanager.h"
#include "utility.h"
#include "system.h"

System::System(sf::Uint16 id_, std::string n) : id(id_), name(n) {
  std::stringstream ss;
  ss << "System: " << name << " (id: " << id << ") created";
  LogConsole(ss.str());
}

System::~System() {
  std::stringstream ss;
  ss << "System: " << name << "(" << id << ") destroyed";
  LogConsole(ss.str());
}

void System::AddPlayer(Player* p) {
  for( size_t i = 0; i < players.size(); i++ ) {
    if( players[i] == p ) {
      return;
    }
  }

  players.push_back(p);

  for( size_t i = 0; i < objects.size(); i++ ) {
    if( !objects[i].first->IsDeleted() ) {
      sf::Packet packet;
      objects[i].first->FillFullPacket(packet);
      p->SendPacket(packet);
    }
  }
}

void System::AddObject(Object* o) {
  for( size_t i = 0; i < objects.size(); i++ ) {
    if( objects[i].first == o ) {
      return;
    }
  }

  objects.push_back( std::make_pair<Object*,char>(o,DIRTY_FULL) );
}

void System::RemoveDeletedPlayers() {
  for( std::vector<Player*>::iterator i = players.begin(); i != players.end(); i++ ) {
    if( (*i)->IsDeleted() ) {
      players.erase(i);
      return;
    }
  }
}

void System::RemoveObject(Object* o) {
  for( std::vector< std::pair<Object*,char> >::iterator i = objects.begin(); i != objects.end(); i++ ) {
    if( (*i).first == o ) {
      objects.erase(i);
      return;
    }
  }
}

void System::Tick(float time) {
  RemoveDeletedPlayers();

  for( size_t i = 0; i < objects.size(); i++ ) {
    char state = objects[i].second;

    if( objects[i].first->IsDeleted() ) {
      state = DELETED;
    } else if( objects[i].first->IsDirty() ) {
      state = DIRTY_PARTIAL;
    } else if( objects[i].first->IsFresh() ) {
      state = DIRTY_FULL;
    }

    sf::Packet p;
    switch( state ) {
      case DIRTY_PARTIAL:
        objects[i].first->FillPartialPacket(p);

        for( size_t j = 0; j < players.size(); j++ ) {
          players[j]->SendPacket(p);
        }

        objects[i].second = CLEAN;
        objects[i].first->ClearDirty();
        break;
      case DIRTY_FULL:
        objects[i].first->FillFullPacket(p);

        for( size_t j = 0; j < players.size(); j++ ) {
          players[j]->SendPacket(p);
        }

        objects[i].second = CLEAN;
        objects[i].first->ClearDirty();
        objects[i].first->ClearFresh();
        break;
      case DELETED:
        p << (sf::Uint16)SERVER_OBJECT << objects[i].first->GetId() << (sf::Uint16)OBJECT_REMOVE;

        for( size_t j = 0; j < players.size(); j++ ) {
          players[j]->SendPacket(p);
        }

        RemoveObject(objects[i].first);
        break;
    }
  }
}
