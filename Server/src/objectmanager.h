#ifndef OBJECTMANAGER_H_INCLUDED
#define OBJECTMANAGER_H_INCLUDED

#include <list>
#include <map>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "objects/objects.h"
#include "objects/object.h"
#include "system.h"

typedef boost::function<ObjectPtr ()> ObjectFactory;
typedef std::map< sf::Uint16, ObjectFactory > FactoryMap;

// Template object factory
template <class T>
ObjectPtr Create() { return ObjectPtr( boost::make_shared<T>() ); }

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

    ObjectPtr GetObjectById( sf::Uint16 id ) const;
    ObjectPtr CreateObject( sf::Uint16 type );

    SystemPtr GetSystemById( sf::Uint16 id ) const;

    void Tick( float time );

    static void AddFactory( sf::Uint16, ObjectFactory );

    sf::Uint16 NewID();

  private:

		void CreateSystem( std::string name );

		void LoadObjects();

    bool m_objects_loaded;

    std::list<ObjectPtr> m_objects;
    std::list<SystemPtr> m_systems;
    sf::Uint16 m_lastObjectId;
    sf::Uint16 m_lastSystemId;

    static FactoryMap* m_factories;
    boost::scoped_ptr<FactoryMap> m_factories_auto_delete;
};

typedef boost::shared_ptr<ObjectManager> ObjectManagerPtr;

#endif // OBJECTMANAGER_H_INCLUDED
