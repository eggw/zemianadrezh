#ifndef USERINTERFACE_H_INCLUDED
#define USERINTERFACE_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "Interface/ChatBox.h"
#include "Interface/HotbarInterface.h"

//A wrapper class that contains all UI elements (such as
//the hotbar, chatbox, etc.)

class NetworkManagerClient;

class UserInterface {
  public:
	UserInterface(sf::RenderWindow& _window,
				  NetworkManagerClient& _n,
				  std::string _name);

	void getInput(sf::Event& _event);
	void update(int _timeslice);
	void draw(sf::RenderStates states = sf::RenderStates::Default) const;

  private:
  	//Functions -----------------------------------
  	void handleIncomingMessages();
	void handleOutgoingMessages();
  	//---------------------------------------------

  	//Data members --------------------------------
  	sf::RenderWindow&                   m_window;
  	NetworkManagerClient&               m_networkManager;
  	ChatBox                             m_chatBox;
	HotbarInterface                     m_hotbarInterface;
	//---------------------------------------------
};

#endif // USERINTERFACE_H_INCLUDED
