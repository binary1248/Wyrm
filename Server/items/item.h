#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <string>

class Item {
  public:
    Item(std::string n, std::string t);
    ~Item();
  private:
    bool equipable;

    std::string name;
    std::string type;
};

#endif // ITEM_H_INCLUDED
