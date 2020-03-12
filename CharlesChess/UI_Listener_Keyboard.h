#pragma once

#include "SFML/Graphics.hpp"

class UI_Board;

class UI_Listener_Keyboard
{
public:
	UI_Listener_Keyboard(UI_Board* aChessBoard) : myChessBoard(aChessBoard) {}

	void Update(sf::RenderWindow& aWindow);

private:
	void HandleNumberKeyPress(sf::RenderWindow& aWindow, sf::Keyboard::Key aKey, bool& aLatch);
	
	void OnAKeyPressed(sf::RenderWindow& aWindow);
	void OnSKeyPressed(sf::RenderWindow& aWindow);
	void OnDKeyPressed(sf::RenderWindow& aWindow);
	void OnPKeyPressed(sf::RenderWindow& aWindow);
	void OnCKeyPressed(sf::RenderWindow& aWindow);

	void OnBackspaceKeyPressed(sf::RenderWindow& aWindow);
	void OnEscapeKeyPressed(sf::RenderWindow& aWindow);

	void OnNumberKeyPressed(sf::RenderWindow& aWindow, sf::Keyboard::Key aKey);

	UI_Board* myChessBoard;
};

