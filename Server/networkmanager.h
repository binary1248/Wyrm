#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <vector>
#include <SFML/Network.hpp>

class Game;

class ClientSocket {
  public:
    ClientSocket(sf::TcpSocket* s);
    ~ClientSocket();

    void Send(sf::Packet& p);
    void Receive(sf::Packet& p);

    sf::TcpSocket* GetSocket();
    sf::Uint16 GetId();
    bool IsHalfOpen();

    void SetId(sf::Uint16 id);
    void SetHalfOpen(bool half_open);
  private:
    sf::TcpSocket* s;
    sf::Uint16 id;
    bool half_open;
};

class NetworkMessage {
  public:
    NetworkMessage(sf::Packet& p);
    NetworkMessage(const char* data, std::size_t size);

    ~NetworkMessage();

    void Send(ClientSocket* s);
    void Receive(ClientSocket* s);

    const char* GetPayload();
    std::size_t GetPayloadSize();
  private:
    void ConstructPacket();
    void ParsePacket();

    sf::Uint16 type;
    sf::Uint16 subtype;
    char* payload;
    std::size_t payload_size;

    sf::Packet packet;
};

class NetworkManager {
  public:
    NetworkManager(Game* g);
    ~NetworkManager();

    void Tick(float time);

    bool IsListening();

    //void RemoveClient(sf::Uint16 id);
  private:
    bool listening;
    void HandlePacket(sf::Packet p, sf::Uint16 id);
    void AcceptSocket();
    void HandleSockets();

    void ClientDisconnect(sf::TcpSocket& client, ClientSocket* pair);
    void ClientConnect(sf::TcpSocket* Client);
    bool ClientAuth(sf::TcpSocket& client, ClientSocket* pair, sf::Packet& packet);

    Game* game;

    sf::TcpListener sock_listener;
    sf::SocketSelector selector;
    std::vector<ClientSocket*> clients;
    std::vector<ClientSocket*>::iterator iter;
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
