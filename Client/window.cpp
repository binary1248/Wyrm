#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFGUI/GUI.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Widget.hpp>

#include "window.h"

namespace sfg {

Window::Window( const sf::FloatRect &rect_, const sf::String &text, const std::string &id ) : Widget(rect, id) {

  flags = HAS_TITLE_BAR | HAS_CLOSE_BUTTON;

  close_focus = false;

  rect = rect_;

  background = sf::Shape::Rectangle(rect, sf::Color(166,166,166,255), 1.0f, sf::Color(111,111,111,255));
  title = Label::Create( sf::Vector2f( rect.Left, rect.Top ), text, text );
  title->SetPadding(sf::Vector2f(2,2));
  title_background = sf::Shape::Rectangle( sf::FloatRect(rect.Left,rect.Top,rect.Width,title->GetRect().Height + title->GetPadding().y*3),
                                           sf::Color(122,122,122,255),
                                           1.0f,
                                           sf::Color(111,111,111,255));

  float thickness = 1.2f;
  float size = title->GetRect().Height + title->GetPadding().y * 3.0f;
  title_bar_height = size;
  close_button.AddPoint(00,00);
  close_button.AddPoint(thickness,00);
  close_button.AddPoint(size/2,size/2-thickness);
  close_button.AddPoint(size-thickness,00);
  close_button.AddPoint(size,00);
  close_button.AddPoint(size,thickness);
  close_button.AddPoint(size/2+thickness,size/2);
  close_button.AddPoint(size,size-thickness);
  close_button.AddPoint(size,size);
  close_button.AddPoint(size-thickness,size);
  close_button.AddPoint(size/2,size/2+thickness);
  close_button.AddPoint(thickness,size);
  close_button.AddPoint(00,size);
  close_button.AddPoint(00,size-thickness);
  close_button.AddPoint(size/2-thickness,size/2);
  close_button.AddPoint(00,thickness);
  close_button.SetColor(sf::Color(255,255,255));
  close_button.SetPosition(rect.Left+rect.Width-size,rect.Top);

  close_button_background = sf::Shape::Rectangle(sf::FloatRect(rect.Left+rect.Width-size,rect.Top,size,size),
                                                 sf::Color(60,60,60,127),
                                                 1.0f,
                                                 sf::Color(111,111,111,255));
}

Window::Ptr Window::Create( const sf::FloatRect &rect, const sf::String &text, const std::string &id ) {
  return Ptr( new Window( rect, text, id ) );
}

bool Window::AddWidget(Widget::Ptr widget) {
  //widget->SetPosition( widget->GetPosition() + GetPosition() + sf::Vector2f(0,title_bar_height+2) );
  return Widget::AddWidget(widget);
}

bool Window::RenderImpl( sf::RenderTarget &target ) const {

  target.Draw(background);

  if(flags & HAS_TITLE_BAR) {
    target.Draw(title_background);
    title->Render(target);

    if(flags & HAS_CLOSE_BUTTON) {
      target.Draw(close_button_background);
      target.Draw(close_button);
    }

  }

  return true;
}

bool Window::OnEvent( const sf::Event &event ) {
  std::cout << event.MouseMove.X << " " << event.MouseMove.Y << std::endl;
  if( event.Type == sf::Event::MouseMoved ) {
    if( event.MouseMove.X >= rect.Left && event.MouseMove.X < rect.Left+rect.Width) {
      if( event.MouseMove.Y >= rect.Top && event.MouseMove.Y < rect.Top+rect.Height) {
        // Window has mouse focus
        if( event.MouseMove.Y < rect.Top+title_bar_height) {
          // Title bar has mouse focus
          if( event.MouseMove.X >= rect.Left + rect.Width - title_bar_height) {
            // Close button has focus
            close_focus = true;
            close_button_background.SetColor(sf::Color(255,255,255,127));
            return false;
          }
        }
      }
    }
  }
  close_focus = false;
  close_button_background.SetColor(sf::Color(60,60,60,127));

  return false;
}

Window::ResizeResult Window::OnResize( const sf::FloatRect &newrect ) {

  return Proceed;
}

void Window::OnFocusChange( bool state ) {

}

}
