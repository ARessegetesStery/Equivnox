#include "eqxpch.h"

#include "LineSeg.h"
#include "Config/config.h"

namespace EQX
{
	extern TGAColor blendTGAColor(TGAColor fore, TGAColor back, double foreAmp);

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
			/*
			this->end.pos.x = s.pos.x;
			this->end.pos.y = s.pos.y;
			this->end.pos.z = s.pos.z;
			this->start.pos.x = e.pos.x;
			this->start.pos.y = e.pos.y;
			this->start.pos.z = e.pos.z;
			*/
		}
		else
		{
			this->end.pos = e.pos;
			this->start.pos = s.pos;
			/*
			this->start.pos.x = s.pos.x;
			this->start.pos.y = s.pos.y;
			this->start.pos.z = e.pos.z;
			this->end.pos.x = e.pos.x;
			this->end.pos.y = e.pos.y;
			this->end.pos.z = s.pos.z;
			*/
		}
		if (abs(start.pos.x - end.pos.x) < DOUBLE_PREC)
			k = SLOPE_MAX + 1;
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
					Vector2 center(x + xPace / 2.0, y + yPace / 2.0);
					double coeff = pixelAmp(*this, center);
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
	double P2LDistance(LineSeg& l, Vertex point)
	{
		Vector2 p(point.pos.x, point.pos.y);
		if (l.k > SLOPE_MAX)
			return abs(p.x - l.start.pos.x);
		else
		{
			Vector2 start(l.start.pos.x, l.start.pos.y);
			double d = abs((-p.y + start.y) * (l.kSign) + abs(l.k) * (p.x - start.x));
			return d * InvSqrt(float(l.k * l.k + 1)); // avoid calculating square root
		}
	}


	double pixelAmp(LineSeg& l, Vertex p)
	{
		double distance = P2LDistance(l, p);
		if (distance > 1.0)
			return 0.0;
		else
			return exp(- 4 * pow(distance, 3));
	}

}