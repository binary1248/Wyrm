#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <vector>
#include <memory>

#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

class GUI {
	public:
		GUI( std::shared_ptr<sf::RenderWindow> window );
		~GUI();

		void Draw( sf::RenderWindow& target );
		bool HandleEvent( sf::Event& event );

		void AddWidget( sfg::Widget::Ptr widget );
		sfg::Widget::Ptr FindWidget( const std::string& id );

		static sf::FloatRect CenterRect( sf::RenderWindow& window, float width, float height );

	private:
		sfg::SFGUI m_sfgui;

		std::vector<sfg::Widget::Ptr> widgets;

		sfg::Desktop m_desktop;
};

typedef std::shared_ptr<GUI> GUIPtr;

#endif // GUI_HPP_INCLUDED
