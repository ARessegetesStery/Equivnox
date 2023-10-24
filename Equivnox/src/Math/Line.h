#pragma once

#include <CoreMinimal.h>

#include "Math.h"

namespace EQX {

	/**
	 * The direction need to be of unit length
	 * Default set to be the Z-axis
	 * Note that the direction may be parallel but opposite
	 */
	class Line
	{
	private:
		Vector3 point;
		Vector3 direction;
		
	public:
		Line();
		Line(Vec3 point, Vec3 direction);
		Line(const Line&);

		inline Vec3 GetPoint() const { return point; }
		inline Vec3 GetDirection() const { return direction; }
		inline void SetPoint(Vec3 p) { point = p; }
		inline void SetDirection(Vec3 dir) { direction = dir.Normalize(); }

		bool operator== (const Line&) const;
	};

	bool IsPointOnLine(const Vec3&, const Line&);

}
