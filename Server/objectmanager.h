#ifndef OBJECTMANAGER_H_INCLUDED
#define OBJECTMANAGER_H_INCLUDED

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "objects/object.h"

class ObjectManager {
  public:
    ObjectManager();
    ~ObjectManager();

    void AddObject(Object* o);
    void RemoveObjectById(sf::Uint16 id);
    Object* GetObjectById(sf::Uint16 id);
    Object* CreateObject(sf::Uint16 type);
    void ClearObjects();

    void SendPacketToObjectById(sf::Uint16 id, sf::Packet p);

    void Tick(float time);

    void SendFullUpdate();
    void SendPartialUpdate();
    void SendStateToPlayerById(sf::Uint16 id);
  private:
    std::vector<Object*> objects;
    sf::Uint16 lastId;
};

extern ObjectManager objectmanager;

#endif // OBJECTMANAGER_H_INCLUDED
