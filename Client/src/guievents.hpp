#ifndef GUIEVENTS_HPP_INCLUDED
#define GUIEVENTS_HPP_INCLUDED

#include <gui.hpp>

void OnLoadGUI( GUI* gui, sf::RenderWindow& window );

void OnQuitClicked();
void OnLoginClicked();
void OnMenuClicked();
void OnReturnToGameClicked();
void OnOptionsClicked();
void OnOptionsOKClicked();
void OnOptionsCancelClicked();
void OnShipClicked();
void OnShipOKClicked();

#endif // GUIEVENTS_HPP_INCLUDED
