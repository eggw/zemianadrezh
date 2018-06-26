#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <memory>

#include <SFML/Graphics.hpp>

#include "Singleton.h"

class ProgramState;

class Program : public Singleton<Program> {

	public:
		Program();
		~Program();

		void init();
		void gameLoop();

		void pushState_Play();
		void pushState_Pause();
		void popState();

		void closeWindow();

		std::vector<std::unique_ptr<ProgramState>> m_states;
		std::unique_ptr<sf::RenderWindow> m_window;
};

#endif // PROGRAM_H_INCLUDED
