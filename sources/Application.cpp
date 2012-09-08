
/*
 *  Application.cpp
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

#include "Application.hpp"
#include "ResourcePath.hpp"
#include <Thor/Shapes.hpp>
#include <sstream>
#include <ctime>

namespace {
	const std::string info =
	std::string("Press P/M to zoom in and out\n") +
	"Press O/L to increase/decrease the fractal rendering precision\n" +
	"Press R to go back to the original view\n" +
	"Press S to take a screenshot of the current view\n" +
	"Press H to hide/show the information panels\n" +
	"Use arrow keys to move in the fractal";
	
	static const sf::Color lightBlue(85, 157, 254);
	static const sf::Color transparentGrey(30, 30, 30, 180);
}

template <typename T>
std::string ftostr(const T& obj)
{
	std::ostringstream ss;
	ss << obj;
	return ss.str();
}

Application::Application(sf::RenderWindow& window) :
m_window(window),
m_textFont(),
m_infoText(),
m_fractalInfoText(),
m_performancesInfoText(),
m_infoShape(),
m_fractalInfoShape(),
m_performancesInfoShape(),
m_cameraSoundBuffer(),
m_callbackSystem(),
m_actionsTable(window),
m_fractalRenderer(m_window.getSize().x, m_window.getSize().y),
m_panelsAreVisible(true)
{
	m_textFont.loadFromFile(resourcePath() + "sansation.ttf");
	m_cameraSoundBuffer.loadFromFile(resourcePath() + "camera.wav");
	
	m_infoText.setCharacterSize(18);
	m_infoText.setStyle(sf::Text::Regular);
	m_infoText.setFont(m_textFont);
	m_infoText.setColor(lightBlue);
	m_infoText.setPosition(10, 10);
	m_infoText.setString(info);
	
	sf::Vector2f infoSize = sf::Vector2f(m_infoText.getGlobalBounds().width + 25,
										 m_infoText.getGlobalBounds().height + 25);
	m_infoShape = thor::Shapes::roundedRect(infoSize , 5, transparentGrey);
	m_infoShape.setPosition(-5, -5);
	
	m_performancesInfoText.setCharacterSize(18);
	m_performancesInfoText.setStyle(sf::Text::Regular);
	m_performancesInfoText.setFont(m_textFont);
	m_performancesInfoText.setColor(lightBlue);
	m_performancesInfoText.setString("Fractal rendered in 0 ms");
	m_performancesInfoText.setPosition(m_window.getSize().x - m_performancesInfoText.getLocalBounds().width - 10, 10);
	
	sf::Vector2f perfSize = sf::Vector2f(m_performancesInfoText.getGlobalBounds().width + 25,
										 m_performancesInfoText.getGlobalBounds().height + 25);
	
	m_performancesInfoShape = thor::Shapes::roundedRect(perfSize , 5, transparentGrey);
	sf::Vector2f perfPos = m_performancesInfoText.getPosition();
	perfPos.x -= 5;
	perfPos.y -= 15;
	m_performancesInfoShape.setPosition(perfPos);
	
	int zoom_stat = m_fractalRenderer.getZoom();
	int resolution_stat = m_fractalRenderer.getResolution();
	double xpos_stat = m_fractalRenderer.getNormalizedPosition().x;
	double ypos_stat = m_fractalRenderer.getNormalizedPosition().y;
	m_fractalInfoText.setCharacterSize(18);
	m_fractalInfoText.setStyle(sf::Text::Regular);
	m_fractalInfoText.setFont(m_textFont);
	m_fractalInfoText.setColor(lightBlue);
	m_fractalInfoText.setString(std::string("Rendering parameters\n") +
								"Zoom: x" + ftostr(zoom_stat) + "\n" +
								"Precision level: " + ftostr(resolution_stat) + "\n" +
								"Position: " + ftostr(xpos_stat) + " ; " + ftostr(ypos_stat));
	m_fractalInfoText.setPosition(10, m_window.getSize().y - m_fractalInfoText.getLocalBounds().height - 10);
	
	sf::Vector2f finfoSize = sf::Vector2f(m_fractalInfoText.getGlobalBounds().width + 25,
										 m_fractalInfoText.getGlobalBounds().height + 25);
	
	m_fractalInfoShape = thor::Shapes::roundedRect(finfoSize , 5, transparentGrey);
	sf::Vector2f finfoPos = m_fractalInfoText.getPosition();
	finfoPos.x -= 15;
	//perfPos.y -= 15;
	m_fractalInfoShape.setPosition(finfoPos);
	
	// Define actions
	m_actionsTable["exit"] = thor::Action(sf::Keyboard::Escape, thor::Action::PressOnce) or thor::Action(sf::Event::Closed);
	
	m_actionsTable["reset view"] = thor::Action(sf::Keyboard::R, thor::Action::PressOnce);
	m_actionsTable["screenshot"] = thor::Action(sf::Keyboard::S, thor::Action::PressOnce);
	m_actionsTable["toggle panels"] = thor::Action(sf::Keyboard::H, thor::Action::PressOnce);
	m_actionsTable["zoom in"] = thor::Action(sf::Keyboard::P, thor::Action::PressOnce);
	m_actionsTable["zoom out"] = thor::Action(sf::Keyboard::M, thor::Action::PressOnce);
	m_actionsTable["increase resolution"] = thor::Action(sf::Keyboard::O, thor::Action::PressOnce);
	m_actionsTable["decrease resolution"] = thor::Action(sf::Keyboard::L, thor::Action::PressOnce);
	
	m_actionsTable["move left"] = thor::Action(sf::Keyboard::Left, thor::Action::PressOnce);
	m_actionsTable["move up"] = thor::Action(sf::Keyboard::Up, thor::Action::PressOnce);
	m_actionsTable["move right"] = thor::Action(sf::Keyboard::Right, thor::Action::PressOnce);
	m_actionsTable["move down"] = thor::Action(sf::Keyboard::Down, thor::Action::PressOnce);
	
	// Connect callbacks
	m_callbackSystem.connect("exit", std::bind(&Application::terminate, this));
	
	m_callbackSystem.connect("reset view", std::bind(&Application::resetView, this));
	m_callbackSystem.connect("screenshot", std::bind(&Application::takeScreenshot, this));
	m_callbackSystem.connect("toggle panels", std::bind(&Application::togglePanels, this));
	m_callbackSystem.connect("zoom in", std::bind(&Application::zoomIn, this));
	m_callbackSystem.connect("zoom out", std::bind(&Application::zoomOut, this));
	m_callbackSystem.connect("increase resolution", std::bind(&Application::increaseResolution, this));
	m_callbackSystem.connect("decrease resolution", std::bind(&Application::decreaseResolution, this));
	
	m_callbackSystem.connect("move left", std::bind(&Application::move, this, Left));
	m_callbackSystem.connect("move up", std::bind(&Application::move, this, Up));
	m_callbackSystem.connect("move right", std::bind(&Application::move, this, Right));
	m_callbackSystem.connect("move down", std::bind(&Application::move, this, Down));
	
	m_fractalRenderer.performRendering();
}

Application::~Application(void)
{
	
}

void Application::handleEvents(void)
{
	m_actionsTable.update();
	m_actionsTable.invokeCallbacks(m_callbackSystem);
}

void Application::update(void)
{
	m_performancesInfoText.setString("Fractal rendered in " + ftostr(m_fractalRenderer.getLastRenderingTime().asMilliseconds()) +" ms");
	m_performancesInfoText.setPosition(m_window.getSize().x - m_performancesInfoText.getLocalBounds().width - 10, 10);
	
	sf::Vector2f perfPos = m_performancesInfoText.getPosition();
	perfPos.x -= 5;
	perfPos.y -= 15;
	sf::Vector2f perfSize = sf::Vector2f(m_performancesInfoText.getGlobalBounds().width + 25,
										 m_performancesInfoText.getGlobalBounds().height + 25);
	m_performancesInfoShape = thor::Shapes::roundedRect(perfSize , 5, transparentGrey);
	m_performancesInfoShape.setPosition(perfPos);
	
	sf::Vector2f finfoSize = sf::Vector2f(m_fractalInfoText.getGlobalBounds().width + 25,
										  m_fractalInfoText.getGlobalBounds().height + 25);
	
	m_fractalInfoShape = thor::Shapes::roundedRect(finfoSize , 5, transparentGrey);
	sf::Vector2f finfoPos = m_fractalInfoText.getPosition();
	finfoPos.x -= 15;
	//perfPos.y -= 15;
	m_fractalInfoShape.setPosition(finfoPos);
	
	m_fractalSprite.setTexture(m_fractalRenderer.getTexture());
	
	double zoom_stat = m_fractalRenderer.getZoom();
	int resolution_stat = m_fractalRenderer.getResolution();
	double xpos_stat = m_fractalRenderer.getNormalizedPosition().x;
	double ypos_stat = m_fractalRenderer.getNormalizedPosition().y;
	m_fractalInfoText.setString(std::string("Rendering parameters\n") +
								"Zoom: x" + ftostr(zoom_stat) + "\n" +
								"Precision level: " + ftostr(resolution_stat) + "\n" +
								"Position: " + ftostr(xpos_stat) + " ; " + ftostr(ypos_stat));
}

void Application::draw(void)
{
	m_window.draw(m_fractalSprite);
	
	if (m_panelsAreVisible)
	{
		m_window.draw(m_infoShape);
		m_window.draw(m_infoText);
		m_window.draw(m_performancesInfoShape);
		m_window.draw(m_performancesInfoText);
		m_window.draw(m_fractalInfoShape);
		m_window.draw(m_fractalInfoText);
	}
}

#pragma mark -
#pragma mark Event callbacks

void Application::terminate(void)
{
	m_window.close();
}

void Application::resetView(void)
{
	m_fractalRenderer.setNormalizedPosition(Vector2lf(0.4, 0.5));
	m_fractalRenderer.setResolution(30);
	m_fractalRenderer.setZoom(1.0);
	m_fractalRenderer.performRendering();
}

void Application::takeScreenshot(void)
{
	sf::Sound sound(m_cameraSoundBuffer);
	sound.play();
	
	sf::Image screenshot = m_window.capture();
	screenshot.saveToFile(std::string("screenshot-") + ftostr(time(NULL)) + ".png");
	
	while (sound.getStatus() == sf::Sound::Playing)
		sf::sleep(sf::milliseconds(10));
}

void Application::togglePanels(void)
{
	m_panelsAreVisible = !m_panelsAreVisible;
}

void Application::zoomIn(void)
{
	double zoom = m_fractalRenderer.getZoom();
	
	m_fractalRenderer.setZoom(zoom * 1.3);
	m_fractalRenderer.performRendering();
}

void Application::zoomOut(void)
{
	double zoom = m_fractalRenderer.getZoom();
	
	m_fractalRenderer.setZoom(zoom * 1/1.3);
	m_fractalRenderer.performRendering();
}

void Application::increaseResolution(void)
{
	int resolution = m_fractalRenderer.getResolution();
	int newResolution = resolution * 1.1;
	
	if (newResolution == resolution)
		newResolution++;
	
	m_fractalRenderer.setResolution(newResolution);
	m_fractalRenderer.performRendering();
}

void Application::decreaseResolution(void)
{
	int resolution = m_fractalRenderer.getResolution();
	int newResolution = resolution * 1./1.1;
	
	if (newResolution == resolution)
		newResolution--;
	
	if (newResolution < 1)
		newResolution = 1;
	
	m_fractalRenderer.setResolution(newResolution);
	m_fractalRenderer.performRendering();
}

void Application::move(Direction aDirection)
{
	Vector2lf position = m_fractalRenderer.getNormalizedPosition();
	double zoom = m_fractalRenderer.getZoom();
	double offset = .1 / zoom;
	
	switch (aDirection) {
		case Left:	position.x -= offset;	break;
		case Right:	position.x += offset;	break;
		case Up:	position.y -= offset;	break;
		case Down:	position.y += offset;	break;
		default:	break;
	}
	
	m_fractalRenderer.setNormalizedPosition(position);
	m_fractalRenderer.performRendering();
}

