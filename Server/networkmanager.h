#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <vector>
#include <SFML/Network.hpp>

inline std::string ErrCode(sf::Socket::Status s) {
  switch(s) {
    case sf::TcpSocket::Disconnected:
      return "Disconnected";
    case sf::TcpSocket::Done:
      return "Done";
    case sf::TcpSocket::Error:
      return "Error";
    case sf::TcpSocket::NotReady:
      return "Not Ready";
    default:
      return "Undefined";
  }
}

class Game;

class NetworkManager {
  public:
    NetworkManager();
    ~NetworkManager();

    void Tick(float time);

    bool IsListening();

    void SelectorAdd(sf::TcpSocket* s);
    void SelectorRemove(sf::TcpSocket* s);
    bool SelectorIsReady(sf::TcpSocket* s);

    //void RemoveClient(sf::Uint16 id);
  private:
    bool listening;
    void HandlePacket(sf::Packet p, sf::Uint16 id);
    void AcceptSocket();

    sf::TcpListener sock_listener;
    sf::SocketSelector selector;
    sf::SocketSelector accept_selector;
};

#define PROTOCOL_VER_MAJOR 0.1f
#define PROTOCOL_VER_MINOR 0.1f

enum server_packet_t0{
  OBJECT = 0
};

enum client_packet_t0{
  COMMAND = 0
};

enum server_packet_t1{
  POSITION_UPDATE = 0,
  VELOCITY_UPDATE,
  NEW_OBJECT,
  REMOVE_OBJECT,
  SET_ID
};

enum client_packet_t1{
  CONTROL = 0
};

#endif // NETWORK_H_INCLUDED
