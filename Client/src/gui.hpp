#ifndef GUI_HPP_INCLUDED
#define GUI_HPP_INCLUDED

#include <vector>

#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

class GUI {
  public:
    GUI( std::shared_ptr<sf::RenderWindow> window );
    ~GUI();

    void Draw( sf::RenderWindow& target );
    bool HandleEvent( sf::Event& event );

    void Hide( const std::string& id );
    void Show( const std::string& id );

    void AddWidget( sfg::Widget::Ptr widget, std::string name );

    sfg::Widget::Ptr FindWidget( const std::string& id );
    sfg::Window::Ptr CreateWindow( const std::string& name );

    static sf::FloatRect CenterRect( sf::RenderWindow& window, float width, float height );

  private:
    std::vector<sfg::Window::Ptr> windows;
    std::vector<sfg::Widget::Ptr> widgets;
};

typedef std::shared_ptr<GUI> GUIPtr;

#endif // GUI_HPP_INCLUDED
