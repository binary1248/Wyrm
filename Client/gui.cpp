#include <iostream>
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
  for( std::vector<sfg::Window::Ptr>::iterator i = windows.begin(); i != windows.end(); i++ ) {
    if( (*i)->GetName() == id ) {
      (*i)->Show(false);
      return;
    }
  }
}

void GUI::Show(const std::string& id) {
  for( std::vector<sfg::Window::Ptr>::iterator i = windows.begin(); i != windows.end(); i++ ) {
    if( (*i)->GetName() == id ) {
      (*i)->Show(true);
      return;
    }
  }
}

void GUI::AddWidget(sfg::Widget::Ptr w, std::string name) {
  w->SetName(name);
  widgets.push_back( w );
}

sfg::Widget::Ptr GUI::FindWidget(const std::string& id) {
  sfg::Widget::Ptr p;

  for( std::vector<sfg::Widget::Ptr>::iterator i = widgets.begin(); i != widgets.end(); i++ ) {
    if( (*i)->GetName() == id ) {
      p = (*i);
      break;
    }
  }

  return p;
}

sfg::Window::Ptr GUI::CreateWindow(const std::string& name) {
  for( std::vector<sfg::Window::Ptr>::iterator i = windows.begin(); i != windows.end(); i++ ) {
    if( (*i)->GetName() == name ) {
      (*i)->Show(false);
      std::cout << "Window already exists." << std::endl;
      sfg::Window::Ptr ptr;
      return ptr;
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

GUI::GUI(sf::RenderWindow& w) {
  OnLoadGUI(this, w);
}

GUI::~GUI() {

}

void GUI::Draw(sf::RenderWindow& w) {
  sf::View view = w.GetView();
  w.SetView(w.GetDefaultView());

  for( std::vector<sfg::Window::Ptr>::iterator i = windows.begin(); i != windows.end(); i++ ) {
    (*i)->Expose(w);
  }

  w.SetView(view);
}

bool GUI::HandleEvent(sf::Event& e) {
  for( std::vector<sfg::Window::Ptr>::iterator i = windows.begin(); i != windows.end(); i++ ) {
    if( !(*i)->IsVisible() ) {
      continue;
    }

    if( (*i)->HandleEvent( e ) == sfg::Widget::EatEvent ) {
      return true;
    }
  }

  return false;
}
