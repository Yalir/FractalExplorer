
/*
 *  Application.hpp
 *	Mandelbrot Fractal Explorer Project - Copyright (c) 2012 Lucas Soltic
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held
 *  liable for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute
 *  it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment
 *  in the product documentation would be appreciated but
 *  is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any
 *  source distribution.
 *
 */

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Thor/Events.hpp>
#include "FractalRenderer.hpp"

class Application {
	sf::RenderWindow& m_window;
	sf::Font m_textFont;
	sf::Text m_infoText;
	sf::Text m_fractalInfoText;
	sf::Text m_performancesInfoText;
	sf::ConvexShape m_infoShape;
	sf::ConvexShape m_fractalInfoShape;
	sf::ConvexShape m_performancesInfoShape;
	sf::SoundBuffer m_cameraSoundBuffer;
	
	thor::ActionMap<std::string>::CallbackSystem m_callbackSystem;
	thor::ActionMap<std::string> m_actionsTable;
	
	FractalRenderer m_fractalRenderer;
	sf::Sprite m_fractalSprite;
	
	bool m_panelsAreVisible;
	
	enum Direction {
		Left,
		Right,
		Up,
		Down
	};
	
public:
	Application(sf::RenderWindow& window);
	~Application(void);
	
	void handleEvents(void);
	void update(void);
	void draw(void);

private:
	void terminate(void);
	void resetView(void);
	void takeScreenshot(void);
	void togglePanels(void);
	void zoomIn(void);
	void zoomOut(void);
	void increaseResolution(void);
	void decreaseResolution(void);
	void move(Direction aDirection);
};

#endif
