#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFGUI/GUI.hpp>

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
  sfg::Widget::Ptr p;

  for( std::map<std::string,WidgetSet::Ptr>::iterator i = sets.begin(); i != sets.end(); i++ ) {
    if(!i->second->GetName().compare(id)) {
      i->second->Hide();
      return;
    }
  }
}

sfg::Widget::Ptr GUI::FindWidget(const std::string& id) {
  sfg::Widget::Ptr p;

  for( std::map<std::string,WidgetSet::Ptr>::iterator i = sets.begin(); i != sets.end(); i++ ) {
    p = i->second->FindWidget(id);
    if(p)
      break;
  }

  return p;
}

WidgetSet::Ptr GUI::CreateSet(const sf::FloatRect rect, const std::string name) {
  if(sets.find(name) == sets.end()) {
    WidgetSet::Ptr ptr(WidgetSet::Create( rect, name ));
    sets.insert( std::pair<std::string,WidgetSet::Ptr>(name,ptr) );
    return ptr;
  } else {
    std::cout << "Window already exists." << std::endl;
  }
  WidgetSet::Ptr ptr;
  return ptr;
}

GUI::GUI(sf::RenderWindow& w) {
  OnLoadGUI(this, w);
}

GUI::~GUI() {

}

void GUI::Draw(sf::RenderWindow& w) {
  sf::View view = w.GetView();
  w.SetView(w.GetDefaultView());

  for( std::map<std::string,WidgetSet::Ptr>::iterator i = sets.begin(); i != sets.end(); i++ ) {
    i->second->Draw(w);
  }

  w.SetView(view);
}

bool GUI::HandleEvent(sf::Event& e) {
  for( std::map<std::string,WidgetSet::Ptr>::iterator i = sets.begin(); i != sets.end(); i++ ) {
    if(i->second->IsActive())
      return i->second->HandleEvent(e);
  }
  return false;
}
