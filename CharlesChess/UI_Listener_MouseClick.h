#pragma once

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"

class UI_Board;
class UI_Tile;

class UI_Listener_MouseClick
{
public:
	UI_Listener_MouseClick(UI_Board* aChessBoard);

	void Update(sf::RenderWindow& aWindow);

private:

	UI_Tile* GetClickedTile(sf::RenderWindow& aWindow);
	void OnMouseClick(sf::RenderWindow& aWindow);

	UI_Board* myChessBoard;
};

