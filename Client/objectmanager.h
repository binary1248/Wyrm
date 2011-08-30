#ifndef OBJECTMANAGER_H_INCLUDED
#define OBJECTMANAGER_H_INCLUDED

#include <vector>
#include <boost/function.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "objects/object.h"

// Template object factory
template <class T>
Object* Create(sf::Uint16 id_, sf::Packet& p) { return new T(id_,p); }

// A macro to register new object factories
#define REGISTER_FACTORY(Type,Class) \
class add_factory_##Class { \
  public: \
    add_factory_##Class() { \
      ObjectManager::AddFactory(Type, Create<Class>); \
    } \
}; \
add_factory_##Class _##Class

class ObjectManager {
  public:
    ObjectManager();
    ~ObjectManager();

    void AddObject(Object* o);
    void RemoveObjectById(sf::Uint16 id);
    Object* GetObjectById(sf::Uint16 id);
    void CreateObject(sf::Packet p, sf::Uint16 id);

    void DispatchPacket(sf::Packet p);

    void Tick(float time);

    void DrawAll(sf::RenderWindow& w);

    static void AddFactory(sf::Uint16, boost::function<Object* (sf::Uint16, sf::Packet&)> );
  private:
    std::vector<Object*> objects;

    static std::map< sf::Uint16, boost::function<Object* (sf::Uint16, sf::Packet&)> >* factories;
};

#endif // OBJECTMANAGER_H_INCLUDED
