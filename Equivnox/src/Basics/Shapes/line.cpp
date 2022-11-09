#include "eqxpch.h"

#include "line.h"
#include "Config/config.h"
#include "Basics/Renderer/TGAUtility.h"

namespace EQX
{
	TGAColor ampTGAColor(TGAColor color, double coeff)
	{
		return TGAColor((int)(color.r / coeff),
			(int)(color.g / coeff),
			(int)(color.b / coeff),
			(int)(color.a / coeff));
	}

	line::line() : start(vec2(0,0)), end(vec2(0,0)), k(0) {  }
	
	/**
	 * The line is required to have the start point with smaller x
	 * than the end one.
	 *
	 * @param s intended start point
	 * @param e intended end point
	 */
	line::line(vec2 s, vec2 e) 
	{
		if (e.x < s.x)
		{
			this->end.x = s.x;
			this->end.y = s.y;
			this->start.x = e.x;
			this->start.y = e.y;
		}
		else
		{
			this->start.x = s.x;
			this->start.y = s.y;
			this->end.x = e.x;
			this->end.y = e.y;
		}
		if (abs(start.x - end.x) < DOUBLE_PREC)
			k = SLOPE_MAX + 1;
		else
			k = (start.y - end.y) / (start.x - end.x);
	}

	void line::render(TGAImage& image, TGAColor color)
	{
		int sx = static_cast<int>(this->start.x);
		int sy = static_cast<int>(this->start.y);
		int ex = static_cast<int>(this->end.x);
		int ey = static_cast<int>(this->end.y);
		// std::cout << sx << " " << sy << " " << ex << " " << ey << std::endl;

		int xPace = (ex - sx) / (abs(sx - ex));
		int yPace = (ey - sy) / (abs(sy - ey));

		std::cout << xPace << yPace << std::endl;

		for (int x = sx; x != ex + 1; x += xPace)
		{
			for (int y = sy; y != ey + 1; y += yPace)
			{
				vec2 center(x + xPace / 2.0, y + yPace / 2.0);
				image.set(x, y, ampTGAColor(color, pixelAmp(*this, center)));
				std::cout << x << y << std::endl;
			}
		}

	}

	/**
	 * Calculate approximate point to line distance
	 * 
	 * @param l
	 * @param p
	 * @return distance; SLOPE_MAX is slope is too high
	 */
	double P2LDistance(line& l, vec2 p)
	{
		if (l.k > SLOPE_MAX)
			return abs(p.x - l.start.x);
		else
		{
			vec2 start = l.start;
			double d = abs(p.y - (l.start.y + l.k * (p.x - l.start.x)));
			return d * std::min(1 / l.k, 1.0); // avoid calculating square root
		}
	}


	double pixelAmp(line& l, vec2 p)
	{
		float distance = P2LDistance(l, p);
		if (distance < 0.3)
			return 1.0;
		else if (distance < 1.3)
			return 1.3 - distance;
		else
			return 0.0;
	}

}