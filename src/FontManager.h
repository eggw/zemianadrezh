#ifndef FONTMANAGER_H_INCLUDED
#define FONTMANAGER_H_INCLUDED

#include <SPSS/Util/Singleton.h>
#include <SPSS/System/ResourceManager.h>

#include <SFML/Graphics/Font.hpp>

enum class FONT
{
	ANDY
};

class FontManager : public spss::Singleton<FontManager>,
                    public spss::ResourceManager<FONT, sf::Font> {
  public:
	inline FontManager() {
		auto andy{std::make_unique<sf::Font>()};
		andy->loadFromFile("assets/fonts/andy.ttf");
		add(FONT::ANDY, std::move(andy));
	}
};

#endif // FONTMANAGER_H_INCLUDED
