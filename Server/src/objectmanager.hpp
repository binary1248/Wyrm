#ifndef OBJECTMANAGER_HPP_INCLUDED
#define OBJECTMANAGER_HPP_INCLUDED

#include <list>
#include <map>
#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <objects/objects.hpp>
#include <objects/object.hpp>
#include <system.hpp>

typedef std::function<ObjectPtr ()> ObjectFactory;
typedef std::map< sf::Uint16, ObjectFactory > FactoryMap;

// Template object factory
template <class T>
ObjectPtr Create() {
	return ObjectPtr( std::make_shared<T>() );
}

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

    const ObjectPtr GetObjectById( sf::Uint16 id ) const;
    const ObjectPtr CreateObject( sf::Uint16 type );

    const SystemPtr GetSystemById( sf::Uint16 id ) const;

    void Tick( float time );

    static void AddFactory( sf::Uint16, ObjectFactory );

    sf::Uint16 NewID();

  private:
		void CreateSystem( std::string name, sf::Uint32 background_resource_id );

		void LoadObjects();

    std::list<ObjectPtr> m_objects;
    std::list<SystemPtr> m_systems;
    sf::Uint16 m_lastSystemId;
    sf::Uint16 m_lastObjectId;

    bool m_objects_loaded;

    static FactoryMap* m_factories;
    std::unique_ptr<FactoryMap> m_factories_auto_delete;
};

typedef std::shared_ptr<ObjectManager> ObjectManagerPtr;

#endif // OBJECTMANAGER_HPP_INCLUDED
