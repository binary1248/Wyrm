#include <map>
#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>
#include <SFGUI/GUI.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Editbox.hpp>

#include "gui.h"
#include "network.h"

std::map<std::string,WidgetSet::Ptr> sets;

sf::FloatRect CenterRect(sf::RenderWindow& w, float width, float height) {
  unsigned int margin_h = (w.GetWidth() - width) / 2;
  unsigned int margin_v = (w.GetHeight() - height) / 2;
  return sf::FloatRect(static_cast<float>(margin_h), static_cast<float>(margin_v), width, height);
}

void HideGUI(const std::string& id) {
  std::map<std::string,WidgetSet::Ptr>::iterator i = sets.begin();

  sfg::Widget::Ptr p;

  while(i != sets.end()) {
    if(!i->second->GetName().compare(id)) {
      i->second->Hide();
      return;
    }
    i++;
  }
}

sfg::Widget::Ptr FindWidget(const std::string& id) {
  std::map<std::string,WidgetSet::Ptr>::iterator i = sets.begin();

  sfg::Widget::Ptr p;

  while(i != sets.end()) {
    p = i->second->FindWidget(id);
    if(p)
      break;
    i++;
  }

  return p;
}

WidgetSet::Ptr WidgetSet::Create(const sf::FloatRect rect, const std::string name) {
  WidgetSet::Ptr ptr(new WidgetSet(rect, name));
  if(sets.find(name) == sets.end()) {
    sets.insert(std::pair<std::string,WidgetSet::Ptr>(name,ptr));
  }
  else
    std::cerr << "Window already exists." << std::endl;
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

void OnQuitClicked( sfg::Widget::Ptr /*widget*/ );
void OnLoginClicked( sfg::Widget::Ptr /*widget*/ );

void LoadGUI(sf::RenderWindow& w) {

  sfg::Button::Ptr quit = sfg::Button::Create( sf::FloatRect( 200, 120, 80, 30 ),
                                                 "Quit" );

  sfg::Button::Ptr login = sfg::Button::Create( sf::FloatRect( 100, 120, 80, 30 ),
                                                 "Login" );

  quit->Clicked = sfg::Slot<sfg::Button::ClickSlot>(&OnQuitClicked);
  login->Clicked = sfg::Slot<sfg::Button::ClickSlot>(&OnLoginClicked);

  sfg::Label::Ptr username_label = sfg::Label::Create( sf::Vector2f( 20, 25 ),
                                                       "Username:" );

  sfg::Editbox::Ptr username = sfg::Editbox::Create( sf::FloatRect( 100, 20, 180, 30 ),
                                                     "Username" );

  sfg::Label::Ptr password_label = sfg::Label::Create( sf::Vector2f( 20, 75 ),
                                                       "Password:" );

  sfg::Editbox::Ptr password = sfg::Editbox::Create( sf::FloatRect( 100, 70, 180, 30 ),
                                                     "Password" );

  password->HideText("#");

  WidgetSet::Ptr set = WidgetSet::Create( CenterRect(w, 300, 170), "login_gui" );


  set->Show();

  set->AddWidget(quit);
  set->AddWidget(login);
  set->AddWidget(username);
  set->AddWidget(username_label);
  set->AddWidget(password);
  set->AddWidget(password_label);
}

void DrawGUI(sf::RenderWindow& w) {
  sf::View view = w.GetView();
  w.SetView(w.GetDefaultView());

  for( std::map<std::string,WidgetSet::Ptr>::iterator i = sets.begin(); i != sets.end(); i++ ) {
    i->second->Draw(w);
  }

  //sf::Text text("LoL");
  //text.SetPosition(300,300);
  //w.Draw(text);

  w.SetView(view);
}

bool HandleGUIEvent(sf::Event& e) {
  for( std::map<std::string,WidgetSet::Ptr>::iterator i = sets.begin(); i != sets.end(); i++ ) {
    if(i->second->IsActive())
      return i->second->HandleEvent(e);
  }
  return false;
}

void OnQuitClicked( sfg::Widget::Ptr w ) {
  bRunning = false;
}

void OnLoginClicked( sfg::Widget::Ptr w ) {

  sfg::Widget::Ptr username = (FindWidget("Username"));
  sfg::Widget::Ptr password = (FindWidget("Password"));

  if(!username || !password)
    return;

  if(!networkhandler.Connect(((sfg::Editbox*)username.get())->GetText(), ((sfg::Editbox*)password.get())->GetText()))
    HideGUI("login_gui");
}
