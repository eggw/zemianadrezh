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
		void pushState_Pause();
		void pushState_MPMenu();
		void pushState_MPHostMenu();

		void returnToMainMenu();

		bool localServerInitialised();
		void initialiseLocalServer(bool _joinable);
		void terminateLocalServer();

		void popState();

		void closeWindow();

		std::vector<std::unique_ptr<ProgramState>> m_states;
		std::unique_ptr<sf::RenderWindow> m_window;

	private:
		bool isAtMainMenu();
		std::unique_ptr<Server> m_localServer{nullptr};
};

#endif // PROGRAM_H_INCLUDED
