#include <iostream>
#include <cmath>
#include <SFML/Network.hpp>

#include "player.h"
#include "network.h"
#include "globals.h"

#define clean_angle(a) (((a+90)/180)*M_PI)

Player::Player(sf::Uint16 id_, sf::String n, sf::TcpSocket* s) {
  id = id_;
  name = n;
  connection = s;
  shipId = 0xFFFF;
}

Player::~Player() {
  Disconnect();
}

void Player::SetShip(sf::Uint16 id) {
  shipId = id;
}

void Player::HandlePacket(sf::Packet p) {

}

void Player::SendPacket(sf::Packet p) {
  connection->Send(p);
}

void Player::Disconnect() {
  if(connection) {
    connection->Disconnect();
    //listener.RemoveClient(id);
    delete connection;
    connection = 0;
  }
}
