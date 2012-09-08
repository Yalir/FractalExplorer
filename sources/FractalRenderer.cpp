
/*
 *  FractalRenderer.cpp
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

#include "FractalRenderer.hpp"
#include <tbb/parallel_for.h>
#include <iostream>


FractalRenderer::FractalRenderer(unsigned width, unsigned heigth) :
m_data(NULL),
m_texture(),
m_normalizedPosition(0.4, 0.5),
m_scale(1.0),
m_resolution(30),
m_image_x(width),
m_image_y(heigth),
m_lastRenderingTime(sf::Time::Zero)
{
	m_data = new unsigned char[m_image_x * m_image_y * 4];
	bzero(m_data, m_image_x * m_image_y * 4);
	
	if (m_texture.create(m_image_x, m_image_y))
	{
		m_texture.setSmooth(true);
	}
	else
	{
		std::cout << "texture size is too big for your crapy graphics card" << std::endl;
	}
}

FractalRenderer::~FractalRenderer()
{
	delete m_data;
}

#include <cstdio>
void FractalRenderer::performRendering(void)
{
	printf("data=%p, width=%d, heigth=%d, zoom=%f, resolution=%d, posx=%f, posy=%f\n",
		   m_data, m_image_x, m_image_y, m_scale, m_resolution, m_normalizedPosition.x, m_normalizedPosition.y);
	
	sf::Clock timer;
	parallel_for(tbb::blocked_range2d<unsigned, unsigned>(0, m_image_x, 50, 0, m_image_y, 50),
				 MandelbrotRenderer(m_data, m_image_x, m_image_y, m_scale, m_resolution, m_normalizedPosition));
	
	m_texture.update(m_data);
	m_lastRenderingTime = timer.getElapsedTime();
}

void FractalRenderer::setZoom(double zoom)
{
	m_scale = zoom;
}


void FractalRenderer::setNormalizedPosition(Vector2lf normalizedPosition)
{
	m_normalizedPosition = normalizedPosition;
}


void FractalRenderer::setResolution(int resolution)
{
	m_resolution = resolution;
}


double FractalRenderer::getZoom(void)
{
	return m_scale;
}


const Vector2lf& FractalRenderer::getNormalizedPosition(void)
{
	return m_normalizedPosition;
}


int FractalRenderer::getResolution(void)
{
	return m_resolution;
}

const sf::Time& FractalRenderer::getLastRenderingTime(void)
{
	return m_lastRenderingTime;
}

const sf::Texture& FractalRenderer::getTexture(void)
{
	return m_texture;
}




