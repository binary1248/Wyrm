#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFML/Graphics.hpp>

#include <config.hpp>
#include <guievents.hpp>
#include <network.hpp>
#include <game.hpp>
#include <gui.hpp>

sf::FloatRect GUI::CenterRect( sf::RenderWindow& window, float width, float height ) {
  float margin_h = ( static_cast<float>( window.GetWidth() ) - width ) / 2.f;
  float margin_v = ( static_cast<float>( window.GetHeight() ) - height ) / 2.f;
  return sf::FloatRect( static_cast<float>( margin_h ), static_cast<float>( margin_v ), width, height );
}

void GUI::Hide( const std::string& id ) {
	std::vector<sfg::Window::Ptr>::const_iterator window( windows.begin() );
	std::vector<sfg::Window::Ptr>::const_iterator end( windows.end() );

  for( ; window != end; ++window ) {
		if( (*window)->GetId() == id ) {
      (*window)->Show( false );
      return;
    }
  }
}

void GUI::Show( const std::string& id ) {
	std::vector<sfg::Window::Ptr>::const_iterator window( windows.begin() );
	std::vector<sfg::Window::Ptr>::const_iterator end( windows.end() );

  for( ; window != end; ++window ) {
		if( (*window)->GetId() == id ) {
      (*window)->Show( true );
      return;
    }
  }
}

void GUI::AddWidget( sfg::Widget::Ptr widget, std::string name ) {
  widget->SetId(name);
  widgets.push_back( widget );
}

sfg::Widget::Ptr GUI::FindWidget( const std::string& id ) {
	std::vector<sfg::Widget::Ptr>::const_iterator widget( widgets.begin() );
	std::vector<sfg::Widget::Ptr>::const_iterator end( widgets.end() );

  for( ; widget != end; ++widget ) {
		if( (*widget)->GetId() == id ) {
      return (*widget);
    }
  }

  return sfg::Widget::Ptr();
}

sfg::Window::Ptr GUI::CreateWindow( const std::string& name ) {
	std::vector<sfg::Window::Ptr>::const_iterator window( windows.begin() );
	std::vector<sfg::Window::Ptr>::const_iterator end( windows.end() );

  for( ; window != end; ++window ) {
		if( (*window)->GetId() == name ) {
      (*window)->Show( false );
      std::cout << "Window already exists.\n";
      return sfg::Window::Ptr();
    }
  }

  sfg::Window::Ptr new_window( sfg::Window::Create() );

  new_window->SetId( name );
  new_window->SetTitle( name );
  new_window->SetBorderWidth( 10.f );
  //new_window->SetStyle(sfg::Window::Titlebar | sfg::Window::Background );

  windows.push_back( new_window );

  return new_window;
}

GUI::GUI( std::shared_ptr<sf::RenderWindow> window ) {
  OnLoadGUI( this, *window );
}

GUI::~GUI() {
}

void GUI::Draw( sf::RenderWindow& target ) {
  sf::View view = target.GetView();
  target.SetView( target.GetDefaultView() );

  std::vector<sfg::Window::Ptr>::const_iterator window( windows.begin() );
	std::vector<sfg::Window::Ptr>::const_iterator end( windows.end() );

  for( ; window != end; ++window ) {
    ( *window )->Expose( target );
  }

  target.SetView( view );
}

bool GUI::HandleEvent( sf::Event& event ) {
	std::vector<sfg::Window::Ptr>::const_iterator window( windows.begin() );
	std::vector<sfg::Window::Ptr>::const_iterator end( windows.end() );

  for( ; window != end; ++window ) {
    if( !( *window )->IsVisible() ) {
      continue;
    }

    ( *window )->HandleEvent( event );
  }

  return false;
}
