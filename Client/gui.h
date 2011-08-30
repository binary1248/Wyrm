#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <vector>
#include <SFGUI/Window.hpp>
#include <SFML/Graphics.hpp>


//#include "widgetset.h"

class GUI {
  public:
    GUI(sf::RenderWindow& w);
    ~GUI();
    void Draw(sf::RenderWindow& w);
    bool HandleEvent(sf::Event& e);
    void Hide(const std::string& id);
    void Show(const std::string& id);
    void AddWidget(sfg::Widget::Ptr w, std::string name);
    sfg::Widget::Ptr FindWidget(const std::string& id);
    sfg::Window::Ptr CreateWindow(const std::string& name);
    static sf::FloatRect CenterRect(sf::RenderWindow& w, float width, float height);
  private:
    std::vector<sfg::Window::Ptr> windows;
    std::vector<sfg::Widget::Ptr> widgets;
};

#endif // GUI_H_INCLUDED
