#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <memory>
#include <chrono>

#include <objectmanager.hpp>
#include <playermanager.hpp>
#include <networkmanager.hpp>
#include <resourcemanager.hpp>

class Game {
	public:
		~Game();

		static std::shared_ptr<Game> GetGame();

		int Run();
		void Stop();

		const PlayerManagerPtr& GetPlayerManager() const;
		const ObjectManagerPtr& GetObjectManager() const;
		const NetworkManagerPtr& GetNetworkManager() const;
		const ResourceManagerPtr& GetResourceManager() const;

	private:
		Game();

		void Tick( float time );

		static std::shared_ptr<Game> m_instance;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_last_frame;

		ObjectManagerPtr m_objectmanager;
		PlayerManagerPtr m_playermanager;
		NetworkManagerPtr m_networkmanager;
		ResourceManagerPtr m_resourcemanager;

		bool m_running;
};

typedef std::shared_ptr<Game> GamePtr;

#endif // GAME_HPP_INCLUDED
