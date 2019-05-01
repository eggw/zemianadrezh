#ifndef SYSTEMDRAWING_H_INCLUDED
#define SYSTEMDRAWING_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <entt/entity/registry.hpp>

class SystemDrawing {
  public:
	SystemDrawing(sf::RenderWindow& _window);
	void update(int _timeslice, entt::registry& _reg);

  private:
	sf::RenderWindow& m_window;
};

#endif // SYSTEMDRAWING_H_INCLUDED
