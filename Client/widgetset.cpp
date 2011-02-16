#include <iostream>

#include "widgetset.h"

WidgetSet::Ptr WidgetSet::Create(const sf::FloatRect rect, const std::string name) {
  WidgetSet::Ptr ptr(new WidgetSet(rect, name));
  return ptr;
}

WidgetSet::WidgetSet(const sf::FloatRect rect, const std::string name_) : name(name_) {
  gui = new sfg::GUI( rect );
  active = false;
  background = sf::Shape::Rectangle( rect,
                                     sf::Color(127,127,127,255),
                                     1.0f,
                                     sf::Color(222,222,222,255));
  gui->EnableRenderToImage(false);
}

WidgetSet::~WidgetSet() {
  delete gui;
}

void WidgetSet::AddWidget(sfg::Widget::Ptr widget) {
  widget->SetPosition(widget->GetPosition()+sf::Vector2f( gui->GetRect().Left,
                                                          gui->GetRect().Top ) );
  gui->AddWidget(widget);
}

void WidgetSet::Draw(sf::RenderWindow& w) {
  if(!active)
    return;

  w.Draw(background);

  gui->Render(w);
}

bool WidgetSet::HandleEvent(sf::Event& e) {
  if(!active)
    return false;

  return gui->HandleEvent(e);
}

sf::FloatRect WidgetSet::GetRect() {
  return gui->GetRect();
}

sfg::Widget::Ptr WidgetSet::FindWidget(const std::string& id) {
  return gui->FindWidget(id);
}
