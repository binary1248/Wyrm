#ifndef GUIEVENTS_H_INCLUDED
#define GUIEVENTS_H_INCLUDED

#include "gui.h"

void OnLoadGUI( GUI* gui, sf::RenderWindow& w );

void OnQuitClicked( sfg::Widget::Ptr /*widget*/ );
void OnLoginClicked( sfg::Widget::Ptr /*widget*/ );

#endif // GUIEVENTS_H_INCLUDED
