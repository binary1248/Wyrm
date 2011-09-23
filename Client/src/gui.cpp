#include <iostream>

#include <boost/foreach.hpp>

#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFML/Graphics.hpp>

#include "gui.h"
#include "guievents.h"
#include "network.h"
#include "game.h"

sf::FloatRect GUI::CenterRect(sf::RenderWindow& w, float width, float height) {
  unsigned int margin_h = (w.GetWidth() - width) / 2;
  unsigned int margin_v = (w.GetHeight() - height) / 2;
  return sf::FloatRect(static_cast<float>(margin_h), static_cast<float>(margin_v), width, height);
}

void GUI::Hide(const std::string& id) {
  BOOST_FOREACH( sfg::Window::Ptr window, windows ) {
		if( window->GetName() == id ) {
      window->Show(false);
      return;
    }
  }
}

void GUI::Show(const std::string& id) {
  BOOST_FOREACH( sfg::Window::Ptr window, windows ) {
		if( window->GetName() == id ) {
      window->Show(true);
      return;
    }
  }
}

void GUI::AddWidget(sfg::Widget::Ptr w, std::string name) {
  w->SetName(name);
  widgets.push_back( w );
}

sfg::Widget::Ptr GUI::FindWidget(const std::string& id) {
  BOOST_FOREACH( sfg::Widget::Ptr widget, widgets ) {
		if( widget->GetName() == id ) {
      return widget;
    }
  }

  return sfg::Widget::Ptr();
}

sfg::Window::Ptr GUI::CreateWindow(const std::string& name) {
  BOOST_FOREACH( sfg::Window::Ptr window, windows ) {
		if( window->GetName() == name ) {
      window->Show(false);
      std::cout << "Window already exists." << std::endl;
      return sfg::Window::Ptr();
    }
  }

  sfg::Window::Ptr window( sfg::Window::Create() );
  window->SetName( name );
  window->SetTitle( name );
  window->SetBorderWidth( 10.f );
  //window->SetStyle(sfg::Window::Titlebar | sfg::Window::Background );
  windows.push_back( window );
  return window;
}

GUI::GUI( boost::shared_ptr<sf::RenderWindow> w ) {
  OnLoadGUI( this, *w );
}

GUI::~GUI() {
}

void GUI::Draw(sf::RenderWindow& w) {
  sf::View view = w.GetView();
  w.SetView(w.GetDefaultView());

  BOOST_FOREACH( sfg::Window::Ptr window, windows ) {
    window->Expose(w);
  }

  w.SetView(view);
}

bool GUI::HandleEvent(sf::Event& e) {
  BOOST_FOREACH( sfg::Window::Ptr window, windows ) {
    if( !window->IsVisible() ) {
      continue;
    }

    if( window->HandleEvent( e ) == sfg::Widget::EatEvent ) {
      return true;
    }
  }

  return false;
}
