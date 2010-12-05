#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <vector>

#include <SFML/Network.hpp>

struct sock_id_pair {
  sf::TcpSocket* s;
  sf::Uint16 id;
  bool half_open;
};

class Listener : private sf::Thread {
  public:
    Listener();
    ~Listener();

    //void RemoveClient(sf::Uint16 id);

    bool running;
  private:
    virtual void Run();
    void HandlePacket(sf::Packet p, sf::Uint16 id);
    void AcceptSocket();
    void HandleSockets();

    sf::TcpListener sock_listener;
    sf::SocketSelector selector;
    std::vector<sock_id_pair> clients;
    std::vector<sock_id_pair>::iterator iter;
};

extern Listener listener;

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
