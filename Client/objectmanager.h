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
    void CreateObject(sf::Packet p);

    void DispatchPacket(sf::Packet p);

    void Tick(float time);

    void DrawAll(sf::RenderWindow& w);
  private:
    std::vector<Object*> objects;
};

#endif // OBJECTMANAGER_H_INCLUDED
