#ifndef ITEM_HPP_INCLUDED
#define ITEM_HPP_INCLUDED

#include <string>
#include <memory>

class Item {
  public:
    Item( std::string name, std::string type );
    ~Item();

    const std::string& GetName() const;
    const std::string& GetType() const;
  private:
    bool m_equipable;

    std::string m_name;
    std::string m_type;
};

typedef std::shared_ptr<Item> ItemPtr;
typedef std::weak_ptr<Item> ItemWeakPtr;

#endif // ITEM_HPP_INCLUDED
