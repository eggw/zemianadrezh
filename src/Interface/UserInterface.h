#ifndef USERINTERFACE_H_INCLUDED
#define USERINTERFACE_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "Interface/ChatBox.h"
#include "Interface/HotbarInterface.h"

//A wrapper class that contains all UI elements (such as
//the hotbar, chatbox, etc.)

class NetworkManagerClient;

class UserInterface : public sf::Drawable {
  public:
	UserInterface(NetworkManagerClient& _n,
	              std::string           _name);

	void getInput(sf::Event& _event);
	void update(int _timeslice);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  private:
	//Functions -----------------------------------
	void handleIncomingMessages();
	void handleOutgoingMessages();
	//---------------------------------------------

	//Data members --------------------------------
	mutable sf::RenderTarget*     m_target;
	NetworkManagerClient&         m_networkManager;
	ChatBox                       m_chatBox;
	HotbarInterface               m_hotbarInterface;
	//---------------------------------------------
};

#endif // USERINTERFACE_H_INCLUDED
