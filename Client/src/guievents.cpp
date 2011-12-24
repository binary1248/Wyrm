#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Context.hpp>

#include <game.hpp>
#include <guievents.hpp>

void OnLoadGUI( GUI* gui, sf::RenderWindow& window ) {
////////////////////////////////////////////////////////////////////////////////
// Login Window
////////////////////////////////////////////////////////////////////////////////

  sfg::Window::Ptr login_window( sfg::Window::Create() );
  login_window->SetTitle( "Login" );
  login_window->SetId( "Login" );
  login_window->SetStyle( sfg::Window::Titlebar | sfg::Window::Background );

  sfg::Label::Ptr username_label( sfg::Label::Create("Username:") );
	sfg::Label::Ptr password_label( sfg::Label::Create("Password:") );
	sfg::Label::Ptr address_label( sfg::Label::Create("Server Address:") );

	sfg::Table::Ptr login_table( sfg::Table::Create() );
	login_table->SetColumnSpacings( 10.0f );
	login_table->SetRowSpacings( 10.0f );

	login_table->Attach( username_label, sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), 0 );
	login_table->Attach( password_label, sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), 0 );
	login_table->Attach( address_label, sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), 0 );

	sfg::Entry::Ptr username( sfg::Entry::Create() );
	username->SetRequisition( sf::Vector2f( 100.0f, 0.0f ) );
	//username->SetProperty( "Entry.Normal.CursorColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	username->SetId( "login_username_text" );
	gui->AddWidget( username );

	sfg::Entry::Ptr password( sfg::Entry::Create() );
	password->SetRequisition( sf::Vector2f( 100.0f, 0.0f ) );
	//password->SetProperty( "Entry.Normal.CursorColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	password->SetId( "login_password_text" );
	gui->AddWidget( password );
	password->HideText( '#' );

	sfg::Entry::Ptr address( sfg::Entry::Create() );
	address->SetRequisition( sf::Vector2f( 100.0f, 0.0f ) );
	//address->SetProperty( "Entry.Normal.CursorColor", sf::Color( 0xFF, 0xFF, 0xFF ) );
	address->SetId( "login_server_address_text" );
	gui->AddWidget( address );

	login_table->Attach( username, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ) );
	login_table->Attach( password, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ) );
	login_table->Attach( address, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ) );

	sfg::Button::Ptr quit( sfg::Button::Create( "Quit" ) );
	quit->OnClick.Connect( &OnQuitClicked );
	sfg::Button::Ptr login( sfg::Button::Create( "Login" ) );
	login->OnClick.Connect( &OnLoginClicked );

	sfg::Box::Ptr login_buttons_box( sfg::Box::Create( sfg::Box::HORIZONTAL ) );
	login_buttons_box->SetSpacing( 20.0f );
	login_buttons_box->Pack( login, true );
	login_buttons_box->Pack( quit, true );

	sfg::Box::Ptr login_window_box( sfg::Box::Create( sfg::Box::VERTICAL ) );
	login_window_box->SetSpacing( 20.0f );
	login_window_box->Pack( login_table, true );
	login_window_box->Pack( login_buttons_box, true );
	login_window_box->SetId( "LoginWindowBox" );

	login_window->Add( login_window_box );

  login_window->Show( true );

  {
		sf::FloatRect size = login_window->GetAllocation();
		sf::FloatRect rect = GUI::CenterRect( window, size.Width, size.Height );
		login_window->SetPosition( sf::Vector2f( rect.Left, rect.Top ) );
	}

  gui->AddWidget( login_window );

  address->SetText( "localhost" );

////////////////////////////////////////////////////////////////////////////////
// Loading Window
////////////////////////////////////////////////////////////////////////////////
	sfg::Window::Ptr loading_window( sfg::Window::Create() );
  loading_window->SetStyle( sfg::Window::Background );
  loading_window->SetId( "Loading" );
  loading_window->Show( false );

	sfg::Box::Ptr loading_box( sfg::Box::Create( sfg::Box::VERTICAL, 10.0f ) );

	sfg::ProgressBar::Ptr loading_progress_bar( sfg::ProgressBar::Create() );
	sfg::Label::Ptr loading_resource_name( sfg::Label::Create( "Resource Name" ) );

	loading_box->Pack( loading_progress_bar, true );
	loading_box->Pack( loading_resource_name, true );

	loading_window->Add( loading_box );

	{
		sf::FloatRect size = loading_window->GetAllocation();
		sf::FloatRect rect = GUI::CenterRect( window, size.Width, size.Height );
		loading_window->SetPosition( sf::Vector2f( rect.Left, rect.Top ) );
	}

  gui->AddWidget( loading_window );

////////////////////////////////////////////////////////////////////////////////
// Main HUD Window
////////////////////////////////////////////////////////////////////////////////
	sfg::Window::Ptr hud_menu_bar_window( sfg::Window::Create() );
  hud_menu_bar_window->SetStyle( sfg::Window::NoStyle );
  hud_menu_bar_window->SetId( "HUD_Menu_Bar" );
  hud_menu_bar_window->Show( false );

	sfg::Box::Ptr hud_menu_bar( sfg::Box::Create( sfg::Box::HORIZONTAL, 10.0f ) );

	sfg::Button::Ptr menu_button( sfg::Button::Create("Menu") );
	menu_button->OnClick.Connect( &OnMenuClicked );
	menu_button->SetRequisition( sf::Vector2f( 40.0f, 30.0f ) );
	hud_menu_bar->Pack( menu_button, false );

	hud_menu_bar_window->Add( hud_menu_bar );

  gui->AddWidget( hud_menu_bar_window );

	sfg::Window::Ptr hud_quick_access_window( sfg::Window::Create() );
  hud_quick_access_window->SetStyle( sfg::Window::NoStyle );
  hud_quick_access_window->SetId( "HUD_Quick_Access_Bar" );
  hud_quick_access_window->Show( false );

	sfg::Box::Ptr hud_quick_access_bar( sfg::Box::Create( sfg::Box::HORIZONTAL, 10.0f ) );

	sfg::Button::Ptr ship_button( sfg::Button::Create("Ship") );
	ship_button->OnClick.Connect( &OnShipClicked );
	ship_button->SetRequisition( sf::Vector2f( 40.0f, 30.0f ) );
	hud_quick_access_bar->Pack( ship_button, false );

	hud_quick_access_window->Add( hud_quick_access_bar );
	sf::Vector2f requisition = hud_quick_access_window->GetRequisition();
	hud_quick_access_window->SetPosition( sf::Vector2f( 0.0f, static_cast<float>( window.GetHeight() ) - requisition.y ) );

  gui->AddWidget( hud_quick_access_window );

////////////////////////////////////////////////////////////////////////////////
// Menu Window
////////////////////////////////////////////////////////////////////////////////
	sfg::Window::Ptr menu_window( sfg::Window::Create() );
  menu_window->SetStyle( sfg::Window::Background );
  menu_window->SetId( "Menu" );
  menu_window->Show( false );
	menu_window->SetRequisition( sf::Vector2f( 80.0f, 100.0f ) );

	sfg::Box::Ptr menu_box( sfg::Box::Create( sfg::Box::VERTICAL, 10.0f ) );

	sfg::Button::Ptr return_to_game_button( sfg::Button::Create("Return to game") );
	return_to_game_button->OnClick.Connect( &OnReturnToGameClicked );
	menu_box->Pack( return_to_game_button, true );

	sfg::Button::Ptr options_button( sfg::Button::Create("Options") );
	options_button->OnClick.Connect( &OnOptionsClicked );
	menu_box->Pack( options_button, true );

	sfg::Button::Ptr quit_button( sfg::Button::Create("Quit") );
	quit_button->OnClick.Connect( &OnQuitClicked );
	menu_box->Pack( quit_button, true );

	menu_window->Add( menu_box );

	{
		sf::FloatRect size = menu_window->GetAllocation();
		sf::FloatRect rect = GUI::CenterRect( window, size.Width, size.Height );
		menu_window->SetPosition( sf::Vector2f( rect.Left, rect.Top ) );
	}

  gui->AddWidget( menu_window );

////////////////////////////////////////////////////////////////////////////////
// Options Window
////////////////////////////////////////////////////////////////////////////////
	sfg::Window::Ptr options_window( sfg::Window::Create() );
  options_window->SetStyle( sfg::Window::Titlebar | sfg::Window::Background );
  options_window->SetTitle( "Game Options" );
  options_window->SetId( "Options" );
  options_window->Show( false );

	sfg::Box::Ptr options_box( sfg::Box::Create( sfg::Box::VERTICAL ) );
	sfg::Table::Ptr options_table( sfg::Table::Create() );

	options_box->Pack( options_table, true );

	sfg::Button::Ptr options_ok( sfg::Button::Create( "OK" ) );
	options_ok->OnClick.Connect( &OnOptionsOKClicked );
	options_ok->SetRequisition( sf::Vector2f( 60.0f, 20.0f ) );
	sfg::Button::Ptr options_cancel( sfg::Button::Create( "Cancel" ) );
	options_cancel->OnClick.Connect( &OnOptionsCancelClicked );
	options_cancel->SetRequisition( sf::Vector2f( 60.0f, 20.0f ) );

	sfg::Box::Ptr options_buttons_box( sfg::Box::Create( sfg::Box::HORIZONTAL ) );
	options_buttons_box->SetSpacing( 20.0f );
	options_buttons_box->Pack( options_ok, true );
	options_buttons_box->Pack( options_cancel, true );

	options_box->Pack( options_buttons_box, true );

	options_window->Add( options_box );

	{
		sf::FloatRect size = options_window->GetAllocation();
		sf::FloatRect rect = GUI::CenterRect( window, size.Width, size.Height );
		options_window->SetPosition( sf::Vector2f( rect.Left, rect.Top ) );
	}

  gui->AddWidget( options_window );

////////////////////////////////////////////////////////////////////////////////
// Ship Window
////////////////////////////////////////////////////////////////////////////////
	sfg::Window::Ptr ship_window( sfg::Window::Create() );
  ship_window->SetTitle( "Ship Data" );
  ship_window->SetId( "Ship" );
  ship_window->Show( false );

	sfg::Box::Ptr ship_box( sfg::Box::Create( sfg::Box::VERTICAL ) );
	ship_box->SetRequisition( sf::Vector2f( 400.0f, 300.0f ) );

	//sfg::Window::Ptr inventory_window( gui->CreateWindow("Inventory") );
	//sfg::ListBox::Ptr inventory_list( sfg::ListBox::Create(6, 200) );
	//gui->AddWidget( inventory_list,"inventory_list" );
	//inventory_window->Add( inventory_list );
  //inventory_window->Show(false);

  sfg::Button::Ptr ship_ok( sfg::Button::Create( "OK" ) );
	ship_ok->OnClick.Connect( &OnShipOKClicked );
	ship_ok->SetRequisition( sf::Vector2f( 60.0f, 20.0f ) );

	ship_box->Pack( ship_ok, false );

	ship_window->Add( ship_box );

  {
		sf::FloatRect size = ship_window->GetAllocation();
		sf::FloatRect rect = GUI::CenterRect( window, size.Width, size.Height );
		ship_window->SetPosition( sf::Vector2f( rect.Left, rect.Top ) );
	}

  gui->AddWidget( ship_window );
}

void OnQuitClicked() {
	Game::GetGame()->GetGUI()->FindWidget( "Menu" )->Show( false );
  Game::GetGame()->Stop();
}

void OnLoginClicked() {
  sfg::Widget::Ptr username = Game::GetGame()->GetGUI()->FindWidget( "login_username_text" );
  sfg::Widget::Ptr password = Game::GetGame()->GetGUI()->FindWidget( "login_password_text" );
  sfg::Widget::Ptr address = Game::GetGame()->GetGUI()->FindWidget( "login_server_address_text" );

  if( !username || !password || !address ) {
    return;
  }

  if( !Game::GetGame()->GetNetworkHandler()->Connect( sfg::StaticPointerCast<sfg::Entry>( username )->GetText(),
                                                      sfg::StaticPointerCast<sfg::Entry>( password )->GetText(),
		                                                  sfg::StaticPointerCast<sfg::Entry>( address )->GetText() ) ) {
    Game::GetGame()->GetGUI()->FindWidget( "Login" )->Show( false );
    Game::GetGame()->GetGUI()->FindWidget( "HUD_Menu_Bar" )->Show( true );
    Game::GetGame()->GetGUI()->FindWidget( "HUD_Quick_Access_Bar" )->Show( true );
  }
}

void OnMenuClicked() {
	Game::GetGame()->GetGUI()->FindWidget( "Menu" )->Show( true );
}

void OnReturnToGameClicked() {
	Game::GetGame()->GetGUI()->FindWidget( "Menu" )->Show( false );
}

void OnOptionsClicked() {
	Game::GetGame()->GetGUI()->FindWidget( "Menu" )->Show( false );
	Game::GetGame()->GetGUI()->FindWidget( "Options" )->Show( true );
}

void OnOptionsOKClicked() {
	Game::GetGame()->GetGUI()->FindWidget( "Options" )->Show( false );
}

void OnOptionsCancelClicked() {
	Game::GetGame()->GetGUI()->FindWidget( "Options" )->Show( false );
}

void OnShipClicked() {
	Game::GetGame()->GetGUI()->FindWidget( "Ship" )->Show( true );
}

void OnShipOKClicked() {
	Game::GetGame()->GetGUI()->FindWidget( "Ship" )->Show( false );
}
