#include <items/item.hpp>

Item::Item( std::string name, std::string type ) :
	m_name( name ),
	m_type( type ) {
}

Item::~Item() {
}

const std::string& Item::GetName() const {
	return m_name;
}

const std::string& Item::GetType() const {
	return m_type;
}
