#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <SFML/System/Vector2.hpp>

namespace Util {

	struct WindowInfo{
		WindowInfo(	sf::Vector2u _size,
					sf::Vector2f _position,
					sf::Vector2f _ratio = sf::Vector2f{1,1})
		:	size	{_size},
			position{_position},
			ratio	{_ratio}
		{
		}

		WindowInfo(WindowInfo& _w)
		:	size	{_w.size},
			position{_w.position},
			ratio	{_w.ratio}
		{}

		WindowInfo(const WindowInfo& _w)
		:	size	{_w.size},
			position{_w.position},
			ratio	{_w.ratio}
		{}

		sf::Vector2u size;
		sf::Vector2f position;
		sf::Vector2f ratio;
	};

}

#endif // UTIL_H_INCLUDED
