
/*
 *  MandelbrotRenderer.hpp
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

#ifndef MANDELBROT_RENDERER_HPP
#define MANDELBROT_RENDERER_HPP

#include <SFML/System/Vector2.hpp>
#include <tbb/blocked_range2d.h>

typedef sf::Vector2<double>        Vector2lf;

class MandelbrotRenderer {
	unsigned char *m_pixelBuffer;
	unsigned m_pixelBufferWidth;
	unsigned m_pixelBufferHeigth;
	
	double m_zoom;
	int m_resolution;
	Vector2lf m_normalizedPosition;
	
public:
	MandelbrotRenderer(unsigned char *pixelBuffer, unsigned width, unsigned heigth,
					   double zoom, int resolution, const Vector2lf& normalizedPosition);
	
	void operator()(const tbb::blocked_range2d<unsigned, unsigned>& range) const;
};

#endif
