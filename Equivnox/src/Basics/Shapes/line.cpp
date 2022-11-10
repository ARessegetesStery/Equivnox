#include "eqxpch.h"

#include "line.h"
#include "Config/config.h"

namespace EQX
{
	extern TGAColor blendTGAColor(TGAColor fore, TGAColor back, double foreAmp);

	line::line() : start(vec2(0,0)), end(vec2(0,0)), k(0), kSign(1) {  }
	
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
		kSign = k / abs(k);
	}

	void line::render(TGAImage& image, TGAColor color)
	{
		int sx = static_cast<int>(this->start.x);
		int sy = static_cast<int>(this->start.y);
		int ex = static_cast<int>(this->end.x);
		int ey = static_cast<int>(this->end.y);

		int xPace = 1;
		int yPace = 1;
		if (ey < sy)
			yPace = -1;

		if (sy == ey)
		{
			for (int x = sx; x != ex + xPace; x += xPace)
				image.set(x, sy, blendTGAColor(color, image.get(x, sy), 1.0));
		}

		else
		{
			for (int x = sx; x != ex + xPace; x += xPace)
			{
				for (int y = (int)(sy + k * (x - sx)) - yPace;
					y != (int)(sy + k * (x - sx + 1) + yPace);
					y += yPace)
					// only traverse pixels that will possibly be rendered
				{
					vec2 center(x + xPace / 2.0, y + yPace / 2.0);
					double coeff = pixelAmp(*this, center);
					if (coeff == 0)
						cout << center.x << " " << center.y << " " << P2LDistance(*this, center) << " " << coeff << endl;;
					image.set(x, y, blendTGAColor(color, image.get(x, y), coeff));
				}
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
			double d = abs((-p.y + l.start.y) * (l.kSign) + abs(l.k) * (p.x - l.start.x));
			return d * 1.0 / std::sqrt(l.k * l.k + 1); // avoid calculating square root
		}
	}


	double pixelAmp(line& l, vec2 p)
	{
		double distance = P2LDistance(l, p);
		if (distance > 1.0)
			return 0.0;
		else
			return exp(- 4 * pow(distance, 3));
	}

}