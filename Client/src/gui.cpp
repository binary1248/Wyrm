#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <utility.hpp>
#include <guievents.hpp>
#include <network.hpp>
#include <game.hpp>
#include <gui.hpp>

sf::FloatRect GUI::CenterRect( sf::RenderWindow& window, float width, float height ) {
  float margin_h = ( static_cast<float>( window.GetWidth() ) - width ) / 2.f;
  float margin_v = ( static_cast<float>( window.GetHeight() ) - height ) / 2.f;
  return sf::FloatRect( static_cast<float>( margin_h ), static_cast<float>( margin_v ), width, height );
}

void GUI::AddWidget( sfg::Widget::Ptr widget ) {
  widgets.push_back( widget );

  if( widget->GetName() == "Window" ) {
		m_desktop.Add( widget );
  }
}

sfg::Widget::Ptr GUI::FindWidget( const std::string& id ) {
	std::vector<sfg::Widget::Ptr>::const_iterator widget( widgets.begin() );
	std::vector<sfg::Widget::Ptr>::const_iterator end( widgets.end() );

  for( ; widget != end; ++widget ) {
		if( (*widget)->GetId() == id ) {
      return (*widget);
    }
  }

  LogConsole( "Couldn't find widget: " + string_cast( id ) );

  return sfg::Widget::Ptr();
}

GUI::GUI( std::shared_ptr<sf::RenderWindow> window ) :
	m_desktop( *window ) {
  OnLoadGUI( this, *window );

  //m_desktop.SetProperty( "*", "Size", 20.0f );
}

GUI::~GUI() {
}

void GUI::Draw( sf::RenderWindow& target ) {
  sf::View view = target.GetView();
  target.SetView( target.GetDefaultView() );

  m_desktop.Expose( target );

  target.SetView( view );
}

bool GUI::HandleEvent( sf::Event& event ) {
	m_desktop.HandleEvent( event );

  return false;
}
