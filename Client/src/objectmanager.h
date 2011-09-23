#ifndef OBJECTMANAGER_H_INCLUDED
#define OBJECTMANAGER_H_INCLUDED

#include <vector>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "objects/object.h"

typedef boost::function<ObjectPtr (sf::Uint16, sf::Packet&)> ObjectFactory;
typedef std::map< sf::Uint16, ObjectFactory > FactoryMap;

// Template object factory
template <class T>
ObjectPtr Create( sf::Uint16 id_, sf::Packet& p ) { return boost::make_shared<T>( id_, p ); }

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

    ObjectPtr GetObjectById(sf::Uint16 id);
    void CreateObject(sf::Packet p, sf::Uint16 id);

    void DispatchPacket(sf::Packet p);

    void Tick(float time);

    void DrawAll(sf::RenderWindow& w);

    static void AddFactory(sf::Uint16, boost::function<ObjectPtr (sf::Uint16, sf::Packet&)> );
  private:
    std::vector<ObjectPtr> objects;

    static FactoryMap* factories;
    boost::scoped_ptr<FactoryMap> m_factories_auto_delete;
};

typedef boost::shared_ptr<ObjectManager> ObjectManagerPtr;

#endif // OBJECTMANAGER_H_INCLUDED
