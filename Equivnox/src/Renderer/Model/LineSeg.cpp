#include "eqxpch.h"

#include "LineSeg.h"

namespace EQX
{
	extern TGAColor blendTGAColor(TGAColor fore, TGAColor back, float foreAmp);

	LineSeg::LineSeg() : start(Vector2(0,0)), end(Vector2(0,0)), k(0), kSign(1) {  }
	
	/**
	 * The LineSeg is required to have the start point with smaller x
	 * than the end one.
	 *
	 * @param s intended start point
	 * @param e intended end point
	 */
	LineSeg::LineSeg(Vertex s, Vertex e) 
	{
		if (e.pos.x < s.pos.x)
		{
			this->end.pos = s.pos;
			this->start.pos = e.pos;
		}
		else
		{
			this->end.pos = e.pos;
			this->start.pos = s.pos;
		}
		if (abs(start.pos.x - end.pos.x) < FLOAT_PREC)
			k = (SLOPE_MAX + 1) * (end.pos.x > start.pos.x ? 1 : -1);
		else
			k = (start.pos.y - end.pos.y) / (start.pos.x - end.pos.x);
		kSign = k / abs(k);
	}

	void LineSeg::render(TGAImage& image, TGAColor color)
	{
		int sx = static_cast<int>(this->start.pos.x);
		int sy = static_cast<int>(this->start.pos.y);
		int ex = static_cast<int>(this->end.pos.x);
		int ey = static_cast<int>(this->end.pos.y);

		// no AA
		/*
		bool transpose = false;
		if (abs(this->k) > 1)
		{
			std::swap(sx, sy);
			std::swap(ex, ey);
			if (sx > ex)
			{
				std::swap(sx, ex);
				std::swap(sy, ey);
			}
			transpose = true;
		}

		for (int x = sx; x != ex + 1; x += 1)
		{
			if (transpose)
			{
				int y = std::roundf(1/k * (x - sx)) + sy;
				cout << x << " " << y << endl;
				image.set(y, x, color);
			}
			else
			{
				int y = std::roundf(k * (x - sx)) + sy;
				image.set(x, y, color);
			}
		}

		*/

		// calculate AA
		int xPace = 1;
		int yPace = 1;
		if (ey < sy)
			yPace = -1;

		if (sy == ey)
		{
			for (int x = sx; x != ex + xPace; x += xPace)
				image.set(x, sy, blendTGAColor(color, image.get(x, sy), 1.0));
		}
		else if (abs(k) > SLOPE_MAX)
		{
			for (int y = sy; y != ey + yPace; y += yPace)
				image.set(sx, y, blendTGAColor(color, image.get(sx, y), 1.0));
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
					Vector2 center(x + xPace / 2.0, y + yPace / 2.0);
					float coeff = pixelAmp(*this, center);
#ifdef EQX_DEBUG
					if (coeff == 0)
						cout << center.x << " " << center.y << " " 
							<< P2LDistance(*this, center) << " " << coeff << endl;
#endif
					image.set(x, y, blendTGAColor(color, image.get(x, y), coeff));
				}
			}
		}

	}

	/**
	 * Calculate approximate point to LineSeg distance
	 * 
	 * @param l
	 * @param p
	 * @return distance; SLOPE_MAX is slope is too high
	 */
	float P2LDistance(LineSeg& l, Vertex point)
	{
		Vector2 p(point.pos.x, point.pos.y);
		if (l.k > SLOPE_MAX)
			return abs(p.x - l.start.pos.x);
		else
		{
			Vector2 start(l.start.pos.x, l.start.pos.y);
			float d = abs((-p.y + start.y) * (l.kSign) + abs(l.k) * (p.x - start.x));
			return d * InvSqrt(float(l.k * l.k + 1)); // avoid calculating square root
		}
	}


	float pixelAmp(LineSeg& l, Vertex p)
	{
		float distance = P2LDistance(l, p);
		if (distance > 1.0)
			return 0.0;
		else
			return exp(- 3 * pow(distance, 3));
	}

}