#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Item {
  public:
    Item( std::string name, std::string type );
    ~Item();

    std::string GetName();
    std::string GetType();
  private:
    bool m_equipable;

    std::string m_name;
    std::string m_type;
};

typedef boost::shared_ptr<Item> ItemPtr;
typedef boost::weak_ptr<Item> ItemWeakPtr;

#endif // ITEM_H_INCLUDED
