#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Item {
  public:
    Item(std::string n, std::string t);
    ~Item();

    inline std::string GetName() { return name; }
    inline std::string GetType() { return type; }
  private:
    bool equipable;

    std::string name;
    std::string type;
};

typedef boost::shared_ptr<Item> ItemPtr;
typedef boost::weak_ptr<Item> ItemWeakPtr;

#endif // ITEM_H_INCLUDED
