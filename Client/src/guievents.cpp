#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Context.hpp>

#include <game.hpp>
#include <guievents.hpp>

void OnLoadGUI( GUI* gui, sf::RenderWindow& window ) {
  sfg::Window::Ptr login_window( gui->CreateWindow("Login") );

  sfg::Label::Ptr username_label( sfg::Label::Create("Username:") );
	sfg::Label::Ptr password_label( sfg::Label::Create("Password:") );

  sfg::Box::Ptr login_labels_box( sfg::Box::Create( sfg::Box::Vertical ) );
  login_labels_box->SetSpacing( 20.0f );
	login_labels_box->Pack( username_label, false );
	login_labels_box->Pack( password_label, false );

	sfg::Entry::Ptr username( sfg::Entry::Create() );
	username->SetRequisition( sf::Vector2f( 100.0f, 0.0f ) );
	//username->SetProperty( "Entry.Normal.CursorColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	gui->AddWidget( username,"login_username_text" );

	sfg::Entry::Ptr password( sfg::Entry::Create() );
	password->SetRequisition( sf::Vector2f( 100.0f, 0.0f ) );
	//password->SetProperty( "Entry.Normal.CursorColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	gui->AddWidget( password, "login_password_text" );
	password->HideText( '#' );

	sfg::Box::Ptr login_text_box( sfg::Box::Create( sfg::Box::Vertical ) );
	login_text_box->SetSpacing( 20.0f );
	login_text_box->Pack( username, false );
	login_text_box->Pack( password, false );

	sfg::Box::Ptr login_input_box( sfg::Box::Create( sfg::Box::Horizontal ) );
	login_input_box->SetSpacing( 20.0f );
	login_input_box->Pack( login_labels_box, false );
	login_input_box->Pack( login_text_box, false );

	sfg::Button::Ptr quit( sfg::Button::Create( "Quit" ) );
	quit->OnClick.Connect( &OnQuitClicked );
	sfg::Button::Ptr login( sfg::Button::Create( "Login" ) );
	login->OnClick.Connect( &OnLoginClicked );

	sfg::Box::Ptr login_buttons_box( sfg::Box::Create( sfg::Box::Horizontal ) );
	login_buttons_box->SetSpacing( 20.0f );
	login_buttons_box->Pack( login, true );
	login_buttons_box->Pack( quit, true );

	sfg::Box::Ptr login_window_box( sfg::Box::Create( sfg::Box::Vertical ) );
	login_window_box->SetSpacing( 20.0f );
	login_window_box->Pack( login_input_box, false );
	login_window_box->Pack( login_buttons_box, true );

	login_window->Add( login_window_box );

	sf::FloatRect size = login_window->GetAllocation();
	sf::FloatRect rect = GUI::CenterRect( window, size.Width, size.Height );
	login_window->SetPosition( sf::Vector2f( rect.Left, rect.Top ) );
  login_window->Show( true );


	//sfg::Window::Ptr inventory_window( gui->CreateWindow("Inventory") );
	//sfg::ListBox::Ptr inventory_list( sfg::ListBox::Create(6, 200) );
	//gui->AddWidget( inventory_list,"inventory_list" );
	//inventory_window->Add( inventory_list );
  //inventory_window->Show(false);

}

void OnQuitClicked() {
  Game::GetGame()->Stop();
}

void OnLoginClicked() {
  sfg::Widget::Ptr username = Game::GetGame()->GetGUI()->FindWidget("login_username_text");
  sfg::Widget::Ptr password = Game::GetGame()->GetGUI()->FindWidget("login_password_text");

  if( !username || !password ) {
    return;
  }

  if( !Game::GetGame()->GetNetworkHandler()->Connect(((sfg::Entry*)username.get())->GetText(),
                                                     ((sfg::Entry*)password.get())->GetText())) {
    Game::GetGame()->GetGUI()->Hide("Login");
    Game::GetGame()->GetGUI()->Show("Inventory");
  }
}
