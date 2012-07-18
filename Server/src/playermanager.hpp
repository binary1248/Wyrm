#ifndef PLAYERMANAGER_HPP_INCLUDED
#define PLAYERMANAGER_HPP_INCLUDED

#include <list>
#include <memory>

#include <SFML/Network.hpp>

#include <player.hpp>

class Game;
class Object;

class PlayerManager {
	public:
		PlayerManager();
		~PlayerManager();

		void Tick( float time );

		const PlayerPtr CreatePlayer();

		sf::Uint16 NewID();

	private:
		std::list<PlayerPtr> m_players;
		sf::Uint16 m_lastId;
};

typedef std::shared_ptr<PlayerManager> PlayerManagerPtr;

#endif // PLAYERMANAGER_HPP_INCLUDED
