#ifndef FONTMANAGER_H_INCLUDED
#define FONTMANAGER_H_INCLUDED

#include <SFML/Graphics.hpp>

#include <SPSS/Util/Singleton.h>

class FontManager : public spss::Singleton<FontManager> {
  public:
	FontManager();

	enum class TYPE
	{
		ANDY
	};

	const sf::Font& getFont(FontManager::TYPE _type);
	float           getLineSpacing(FontManager::TYPE _type,
	                               unsigned int      _characterSize);

  private:
	//Data members --------------------------------
	std::map<FontManager::TYPE, sf::Font> m_fonts;
	//---------------------------------------------
};

#endif // FONTMANAGER_H_INCLUDED
