#include "eqxpch.h"

#include "LineSeg.h"

namespace EQX
{
	extern struct TGAColor;

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

		k = GetSlope(start, end);
		kSign = k / abs(k);
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
		if (p.x > l.end.pos.x && l.kSign * (p.y - l.end.pos.y) > 0)
		{
			return 0.5 * (std::abs(p.x - l.end.pos.x) + std::abs(p.y - l.end.pos.y));
		}
		if (p.x < l.start.pos.x && l.kSign * (-p.y + l.start.pos.y) > 0)
		{
			return 0.5 * (std::abs(p.x - l.start.pos.x) + std::abs(p.y - l.start.pos.y));
		}
		if (l.k > SLOPE_MAX)
			return abs(p.x - l.start.pos.x);
		else
		{
			Vector2 start(l.start.pos.x, l.start.pos.y);
			float d = abs((-p.y + start.y) * (l.kSign) + abs(l.k) * (p.x - start.x));
			return d * InvSqrt(float(l.k * l.k + 1)); // avoid calculating square root
		}
	}

	float PixelAmp(LineSeg& l, Vertex p)
	{
		float distance = P2LDistance(l, p);
		if (distance > 1.3)
			return 0.0;
		else
			return exp(3 * -pow(distance, 3));
	}

	float GetSlope(Vertex& start, Vertex& end)
	{
		float k;
		if (abs(start.pos.x - end.pos.x) < FLOAT_PREC)
			k = (SLOPE_MAX + 1) * (end.pos.x > start.pos.x ? 1 : -1);
		else
			k = (start.pos.y - end.pos.y) / (start.pos.x - end.pos.x);
		return k;
	}

}