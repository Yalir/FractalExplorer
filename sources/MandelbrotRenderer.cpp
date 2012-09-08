
/*
 *  MandelbrotRenderer.cpp
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

#include "MandelbrotRenderer.hpp"
#include <iostream>
#include <SFML/System.hpp>

MandelbrotRenderer::MandelbrotRenderer(unsigned char *pixelBuffer, unsigned width, unsigned heigth,
									   double zoom, int resolution, const Vector2lf& normalizedPosition):
m_pixelBuffer(pixelBuffer),
m_pixelBufferWidth(width),
m_pixelBufferHeigth(heigth),
m_zoom(zoom),
m_resolution(resolution),
m_normalizedPosition(normalizedPosition)
{
}


void MandelbrotRenderer::operator()(const tbb::blocked_range2d<unsigned, unsigned>& range) const
{
	const double fractal_left = -2.1;
	const double fractal_right = 0.6;
	const double fractal_bottom = -1.2;
	const double fractal_top = 1.2;
	
	//double zoom_x = m_zoom * m_pixelBufferWidth / (fractal_right - fractal_left);
	double zoom_y = m_zoom * m_pixelBufferHeigth / (fractal_top - fractal_bottom);
	double zoom_x = zoom_y;
	
	int64_t fractal_width = m_pixelBufferWidth * m_zoom;
	int64_t fractal_heigth = m_pixelBufferHeigth * m_zoom;
	
	for (unsigned image_x = range.rows().begin(); image_x != range.rows().end(); image_x++)
	{
		for (unsigned image_y = range.cols().begin(); image_y != range.cols().end(); image_y++)
		{
			int64_t fractal_x = fractal_width * m_normalizedPosition.x - m_pixelBufferWidth / 2 + image_x;
			int64_t fractal_y = fractal_heigth * m_normalizedPosition.y - m_pixelBufferHeigth / 2 + image_y;
			
			double c_r = fractal_x / (double)zoom_x + fractal_left;
			double c_i = fractal_y / (double)zoom_y + fractal_bottom;
			double z_r = 0;
			double z_i = 0;
			double i   = 0;
			
			do{
				double tmp = z_r;
				z_r = z_r * z_r - z_i * z_i + c_r;
				z_i = 2 * tmp * z_i + c_i;
				i++;
			} while (z_r * z_r + z_i * z_i < 4 && i < m_resolution);
			
			if (i == m_resolution)
			{
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 0] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 1] = 54;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 2] = 76;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 3] = 255;
			}
			else
			{
				int val = i * 255 / m_resolution;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 0] = val;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 1] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 2] = 0;
				m_pixelBuffer[(image_y * m_pixelBufferWidth + image_x) * 4 + 3] = 255;
			}
		}
	}
}
