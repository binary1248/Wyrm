#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <map>
#include <SFML/Graphics.hpp>
#include <SFGUI/GUI.hpp>

#include "widgetset.h"

class GUI {
  public:
    GUI(sf::RenderWindow& w);
    ~GUI();
    void Draw(sf::RenderWindow& w);
    bool HandleEvent(sf::Event& e);
    void Hide(const std::string& id);
    sfg::Widget::Ptr FindWidget(const std::string& id);
    WidgetSet::Ptr CreateSet(const sf::FloatRect rect, const std::string name);
    static sf::FloatRect CenterRect(sf::RenderWindow& w, float width, float height);
  private:
    std::map<std::string,WidgetSet::Ptr> sets;
};

#endif // GUI_H_INCLUDED
