#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include <utility>
#include <list>

class Item;
class Player;

class Inventory {
  public:
    Inventory();
    ~Inventory();

    inline void ClearDirty() { isDirty = false; }
    inline bool IsDirty() { return isDirty; }

    void SendUpdate(Player* p);
  private:
    bool isDirty;

    std::list< std::pair<Item*, size_t> > items;
};

#endif // INVENTORY_H_INCLUDED
