#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <SPSS/System/Core.h>
#include <memory>

#include "Server.h"

class Program : public spss::Core {
  public:
	Program(sf::RenderWindow& _window);
	~Program();

	//overloaded from Core; this is because in addition
	//to the functionality Core::run() provides, we also
	//need to update the local server at a specific point
	//(just after updating the client)
	void run();

	void pushState_Play_SP();
	void pushState_Play_MP_Host();
	void pushState_Play_MP_Join(std::string* _ip);

	void pushState_Pause();
	void pushState_MPMenu();
	void pushState_MPHostMenu();
	void pushState_MPJoinMenu();

	void pushState_MPJoinFailed(std::string* _ip);
	void pushState_MPConnectionLost();

	void returnToMainMenu();

	bool localServerInitialised() const;
	void initialiseLocalServer(bool _joinable);

  private:
	//Functions -----------------------------------
	bool isAtMainMenu() const;
	bool isPaused()     const;
	//---------------------------------------------

	//Data members --------------------------------
	std::unique_ptr<Server> m_localServer{nullptr};
	//---------------------------------------------
};

#endif // PROGRAM_H_INCLUDED
