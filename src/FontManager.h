#ifndef FONTMANAGER_H_INCLUDED
#define FONTMANAGER_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "Singleton.h"

class FontManager : public Singleton<FontManager> {

	public:
		FontManager();

		enum class Type
		{
			ANDY
		};

		const sf::Font& getFont(Type _type);
		float getLineSpacing(Type _type, unsigned int _characterSize);

	private:
		std::map<Type, sf::Font> m_fonts;

};

#endif // FONTMANAGER_H_INCLUDED
