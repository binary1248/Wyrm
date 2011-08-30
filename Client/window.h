#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <SFGUI/GUI.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Widget.hpp>

namespace sfg {

class Window : public Widget {

  enum FLAGS {
    HAS_TITLE_BAR = 0x0001,
    HAS_CLOSE_BUTTON = 0x0003
  };

  public:
    typedef boost::shared_ptr<Window>  Ptr;
    static Ptr Create( const sf::FloatRect &rect, const sf::String &text = L"", const std::string &id = "" );

    bool AddWidget(Widget::Ptr widget);

  protected:
		virtual bool RenderImpl( sf::RenderTarget &target ) const;
		virtual bool OnEvent( const sf::Event &event );
		virtual ResizeResult OnResize( const sf::FloatRect &newrect );
		virtual void OnFocusChange( bool state );

  private:
    Window( const sf::FloatRect &rect, const sf::String &text = L"", const std::string &id = "" );

    sf::FloatRect rect;
    float title_bar_height;

    sf::Shape title_background;
    sf::Shape background;
    sf::Shape close_button_background;
    sf::Shape close_button;
    Label::Ptr title;

    sf::Uint16 flags;
    bool close_focus;
};

}

#endif // WINDOW_H_INCLUDED
