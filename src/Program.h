#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <memory>

#include <SFML/Graphics.hpp>

#include "Server.h"
#include "Singleton.h"

class ProgramState;

class Program : public Singleton<Program> {

	public:
		Program();
		~Program();

		void init();
		void gameLoop();

		void pushState_Play_SP();
		void pushState_Play_MP_Host();
		void pushState_Play_MP_Join();


		void pushState_Pause();
		void pushState_MPMenu();
		void pushState_MPHostMenu();
		void pushState_MPJoinMenu();

		void pushState_MPJoinFailed();
		void pushState_MPConnectionLost();

		void returnToMainMenu();

		bool localServerInitialised();
		void initialiseLocalServer(bool _joinable);
		void terminateLocalServer();

		void popState();

		void closeWindow();

		void setIpAddress(const std::string& _ipStr);

		Server* getServer() const;

		std::vector<std::unique_ptr<ProgramState>> m_states;
		std::unique_ptr<sf::RenderWindow> m_window;
	private:
	    void monitorWindowClosing();
		bool isAtMainMenu();
		std::unique_ptr<Server> m_localServer{nullptr};
		//Due to the way menu items work (ProgramState_Menu),
		//they can call Program functions, but they can't pass
		//parameters. Therefore, when a player wants to join
		//a server, we'll set m_ipAddress to the IP they type,
		//so that we can initialise the client with
		//pushState_Play_MP_Join()
		std::string m_ipAddress{"localhost"};
};

#endif // PROGRAM_H_INCLUDED
