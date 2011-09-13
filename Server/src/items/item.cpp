#include "item.h"

Item::Item( std::string name, std::string type ) :
	m_name( name ),
	m_type( type ) {
}

Item::~Item() {
}

std::string Item::GetName() {
	return m_name;
}

std::string Item::GetType() {
	return m_type;
}
