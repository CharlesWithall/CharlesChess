#pragma once

#include "SFML/Graphics.hpp"

class UI_MainMenu_Graphics
{
public:
	UI_MainMenu_Graphics()
	{
		sf::Texture newTexture;
		if (newTexture.loadFromFile("MAINMENU_BUTTON_BACKGROUND.png")) { myButtonBackground = newTexture; }
		if (newTexture.loadFromFile("MAINMENU_TOGGLEBUTTON_BACKGROUND.png")) { myToggleButtonBackground = newTexture; }
		if (newTexture.loadFromFile("MAINMENU_BUTTON_HIGHLIGHT.png")) { myButtonHighlight = newTexture; }

		sf::Font newFont;
		if (newFont.loadFromFile("framd.ttf")) { myFont = newFont; }
	}

	const sf::Texture& GetButtonBackground() const { return myButtonBackground; }
	const sf::Texture& GetToggleButtonBackground() const { return myToggleButtonBackground; }
	const sf::Texture& GetButtonHighlight() const { return myButtonHighlight; }
	const sf::Font& GetFont() const { return myFont; }
	
	void CentreAlignText(sf::Text& someText) const
	{
		const sf::FloatRect& bounds = someText.getLocalBounds();
		someText.setOrigin(bounds.left + (bounds.width / 2), bounds.top + (bounds.height / 2));
	}

private:
	sf::Font myFont;

	sf::Texture myButtonBackground;
	sf::Texture myToggleButtonBackground;
	sf::Texture myButtonHighlight;
};