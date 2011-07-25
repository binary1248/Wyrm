#ifndef OBJECTMANAGER_H_INCLUDED
#define OBJECTMANAGER_H_INCLUDED

#include <vector>
#include <map>
#include <boost/function.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "objects/objects.h"
#include "objects/object.h"
#include "system.h"

// Template object factory
template <class T>
Object* Create() { return new T(); }

// A macro to register new object factories
#define REGISTER_FACTORY(Type,Class) \
class add_factory_##Class { \
  public: \
    add_factory_##Class() { \
      ObjectManager::AddFactory(Type, Create<Class>); \
    } \
}; \
add_factory_##Class _##Class

class Game;
class Player;

class ObjectManager {
  public:
    ObjectManager();
    ~ObjectManager();

    void AddObject(Object* o);
    void RemoveObject(Object* o);
    void RemoveObjectById(sf::Uint16 id);
    Object* GetObjectById(sf::Uint16 id);
    Object* CreateObject(sf::Uint16 type);
    void ClearObjects();

    System* GetSystemById(sf::Uint16 id);
    void CreateSystem(std::string name);
    void ClearSystems();

    void Tick(float time);

    inline sf::Uint16 NewID() { return lastObjectId++; }

    static void AddFactory(sf::Uint16, boost::function<Object* ()> );

    void LoadObjects();
  private:
    bool objects_loaded;
    sf::Clock LastFullUpdate;

    std::vector<Object*> objects;
    std::vector<System*> systems;
    sf::Uint16 lastObjectId;
    sf::Uint16 lastSystemId;

    static std::map< sf::Uint16, boost::function<Object* ()> >* factories;
};

#endif // OBJECTMANAGER_H_INCLUDED
