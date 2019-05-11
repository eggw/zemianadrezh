#ifndef SYSTEMDRAWING_H_INCLUDED
#define SYSTEMDRAWING_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <entt/entity/registry.hpp>

class SystemDrawing {
  public:
	SystemDrawing();
	void draw(entt::registry&   _reg,
	          sf::RenderTarget& _target,
	          sf::RenderStates  _states) const;
};

#endif // SYSTEMDRAWING_H_INCLUDED
