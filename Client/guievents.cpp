#include <SFGUI/GUI.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Editbox.hpp>

#include "guievents.h"
#include "game.h"

void OnLoadGUI( GUI* gui, sf::RenderWindow& w ) {
  sfg::Button::Ptr quit = sfg::Button::Create( sf::FloatRect( 200, 120, 80, 30 ), "Quit" );
  sfg::Button::Ptr login = sfg::Button::Create( sf::FloatRect( 100, 120, 80, 30 ), "Login" );

  quit->Clicked = sfg::Slot<sfg::Button::ClickSlot>(&OnQuitClicked);
  login->Clicked = sfg::Slot<sfg::Button::ClickSlot>(&OnLoginClicked);

  sfg::Label::Ptr username_label = sfg::Label::Create( sf::Vector2f( 20, 25 ), "Username:" );
  sfg::Editbox::Ptr username = sfg::Editbox::Create( sf::FloatRect( 100, 20, 180, 30 ), "Username" );
  sfg::Label::Ptr password_label = sfg::Label::Create( sf::Vector2f( 20, 75 ), "Password:" );
  sfg::Editbox::Ptr password = sfg::Editbox::Create( sf::FloatRect( 100, 70, 180, 30 ), "Password" );

  password->HideText("#");

  WidgetSet::Ptr set = gui->CreateSet( GUI::CenterRect(w, 300, 170), "login_gui" );

  if( set ) {
    set->Show();

    set->AddWidget(quit);
    set->AddWidget(login);
    set->AddWidget(username);
    set->AddWidget(username_label);
    set->AddWidget(password);
    set->AddWidget(password_label);
  }
}

void OnQuitClicked( sfg::Widget::Ptr w ) {
  Game::GetGame()->Stop();
}

void OnLoginClicked( sfg::Widget::Ptr w ) {

  sfg::Widget::Ptr username = (Game::GetGame()->GetGUI()->FindWidget("Username"));
  sfg::Widget::Ptr password = (Game::GetGame()->GetGUI()->FindWidget("Password"));

  if(!username || !password)
    return;

  if(!Game::GetGame()->GetNetworkHandler()->Connect(((sfg::Editbox*)username.get())->GetText(), ((sfg::Editbox*)password.get())->GetText())) {
    Game::GetGame()->GetGUI()->Hide("login_gui");
  }
}
