#include <sstream>
#include <algorithm>

#include <boost/make_shared.hpp>

//#include <SFGUI/ListBox.hpp>

#include "items/item.h"
#include "game.h"
#include "inventory.h"

/*
bool CheckEqual( sfg::ListBox::u32string wstr, std::string str ) {
  if( wstr.size() != str.size() ) {
    return false;
  }

  for( size_t i = 0; i < str.size(); i++ ) {
    if( wstr[i] != (sf::Uint32)(str[i]) ) {
      return false;
    }
  }

  return true;
}
*/

Inventory::Inventory() {

}

Inventory::~Inventory() {
}

void Inventory::HandlePacket( sf::Packet& p ) {
  sf::Uint16 code;
  p >> code;

  sf::String type;
  sf::String name;
  sf::Uint32 amount;

  ItemPtr item;

  switch( code ) {
    case INVENTORY_ADD:
      p >> type;
      p >> name;
      p >> amount;
      item = boost::make_shared<Item>( name.ToAnsiString(), type.ToAnsiString() );
      //AddItem( item, amount );
      break;
    case INVENTORY_CHANGE:
      p >> type;
      p >> name;
      p >> amount;
/*
      for( std::list< std::pair<ItemPtr, size_t> >::iterator i = items.begin(); i != items.end(); i++ ) {
        if( (type.ToAnsiString() == i->first->GetType()) && (name.ToAnsiString() == i->first->GetName()) ) {
          sfg::ListBox* list = (sfg::ListBox*)(Game::GetGame()->GetGUI()->FindWidget("inventory_list").get());
          std::size_t list_entries = list->GetNumEntries();

          std::stringstream ss;
          ss << "[" << i->first->GetType() << "] " << i->first->GetName() << " (x" << i->second << ")";

          size_t j;

          for( j = 0; j < list_entries; j++ ) {
            sfg::ListBox::u32string str = list->GetEntry( j );
            if( CheckEqual( str, ss.str() ) ) {
              list->RemoveEntry( j );
              break;
            }
          }

          i->second = amount;

          ss.str(std::string());
          ss << "[" << i->first->GetType() << "] " << i->first->GetName() << " (x" << i->second << ")";
          list->AddEntry( ss.str(), j );

          break;
        }
      }
*/
      break;
    case INVENTORY_REMOVE:
      p >> type;
      p >> name;
/*
      for( std::list< std::pair<ItemPtr, size_t> >::iterator i = items.begin(); i != items.end(); i++ ) {
        if( (type.ToAnsiString() == i->first->GetType()) && (name.ToAnsiString() == i->first->GetName()) ) {
          sfg::ListBox* list = (sfg::ListBox*)(Game::GetGame()->GetGUI()->FindWidget("inventory_list").get());
          std::size_t list_entries = list->GetNumEntries();

          std::stringstream ss;
          ss << "[" << i->first->GetType() << "] " << i->first->GetName() << " (x" << i->second << ")";

          items.erase( i );

          size_t j;

          for( j = 0; j < list_entries; j++ ) {
            sfg::ListBox::u32string str = list->GetEntry( j );
            if( CheckEqual( str, ss.str() ) ) {
              list->RemoveEntry( j );
              break;
            }
          }

          break;
        }
      }
*/
      break;
    default:
      // std::cout << "Unknown inventory action." << std::endl;
      break;
  }
}

/*
void Inventory::AddItem(ItemPtr item, size_t amount) {
  for( std::list< std::pair<ItemPtr, size_t> >::iterator i = items.begin(); i != items.end(); i++ ) {
    if( item == i->first ) {
      i->second += amount;
      return;
    }
  }

  items.push_back( std::make_pair<ItemPtr, size_t>(item, amount) );

  std::stringstream ss;

  ss << "[" << item->GetType() << "] " << item->GetName() << " (x" << amount << ")";

  //sfg::ListBox* list = (sfg::ListBox*)(Game::GetGame()->GetGUI()->FindWidget("inventory_list").get());

  //list->AddEntry( ss.str() );
}

void Inventory::RemoveItem(ItemPtr item, size_t amount) {
  for( std::list< std::pair<ItemPtr, size_t> >::iterator i = items.begin(); i != items.end(); i++ ) {
    if( item == i->first ) {
      sfg::ListBox* list = (sfg::ListBox*)(Game::GetGame()->GetGUI()->FindWidget("inventory_list").get());
      std::size_t list_entries = list->GetNumEntries();

      std::stringstream ss;
      ss << "[" << i->first->GetType() << "] " << i->first->GetName() << " (x" << i->second << ")";

      size_t j;

      for( j = 0; j < list_entries; j++ ) {
        sfg::ListBox::u32string str = list->GetEntry( j );
        if( CheckEqual( str, ss.str() ) ) {
          list->RemoveEntry( j );
          break;
        }
      }

      if( amount > i->second ) {
        //std::cout << "Tried to remove more items than in inventory." << std::endl;
      } else {
        i->second -= amount;
      }
      if( i->second == 0 ) {
        items.erase( i );
      } else {
        ss.str(std::string());
        ss << "[" << i->first->GetType() << "] " << i->first->GetName() << " (x" << i->second << ")";
        list->AddEntry( ss.str(), j );
      }

      break;
    }
  }
}
*/
