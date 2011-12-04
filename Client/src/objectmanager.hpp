#ifndef OBJECTMANAGER_HPP_INCLUDED
#define OBJECTMANAGER_HPP_INCLUDED

#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <objects/object.hpp>


typedef ObjectPtr ( *ObjectFactory )( sf::Uint16, sf::Packet& );
typedef std::map< sf::Uint16, ObjectFactory > FactoryMap;

// Template object factory
template <class T>
ObjectPtr Create( sf::Uint16 id, sf::Packet& packet ) {
	return ObjectPtr( std::make_shared<T>( id, packet ) );
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

class ObjectManager {
  public:
    ObjectManager();
    ~ObjectManager();

    ObjectPtr GetObjectById( sf::Uint16 id );
    void CreateObject( sf::Packet packet, sf::Uint16 id );

    void DispatchPacket( sf::Packet packet );

    void Tick( float time );

    void DrawAll( sf::RenderWindow& target );

    static void AddFactory( sf::Uint16, ObjectFactory );
  private:
    std::list<ObjectPtr> m_objects;

    static FactoryMap* m_factories;
    std::unique_ptr<FactoryMap> m_factories_auto_delete;
};

typedef std::shared_ptr<ObjectManager> ObjectManagerPtr;

#endif // OBJECTMANAGER_HPP_INCLUDED
