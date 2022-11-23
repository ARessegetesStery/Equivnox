#pragma once

#include "Math/MathHeader.h"

namespace EQX 
{
	/**    
	 * Axes, conforming to right-hand system
	 *     ^ 
	 *     | y
	 *     |
	 *     | /
	 *     |/
	 * ----¡¤-----------> x
	 *    /|
	 * z / |
	 */


	class Vertex
	{
	public: 
		Vector4 pos;
		Vector3 normal;		// Default set to be UNIT_Z
		Vector2 uv;
		Vector3 color;		// Default set to be white; each comp 0~255

	public:
		Vertex(Vector4 pos = Vector4::ZERO, 
			Vector3 normal = Vector3::UNIT_Z, Vector2 uv = Vector2::ZERO);
		Vertex(Vector3 pos, Vector3 normal = Vector3::UNIT_Z, Vector2 uv = Vector2::ZERO);
		Vertex(Vector2 pos,	Vector3 normal = Vector3::UNIT_Z, Vector2 uv = Vector2::ZERO);
	};

	bool operator== (Vertex&, Vertex&);
	bool operator== (Vertex&, const Vertex&);

	/**
	 * return true if l.x is smaller than or equal to r.x
	 * 
	 * @param l vertex supposedly with smaller x
	 * @param r vertex supposedly with larger x
	 */
	bool LefterVertex(const Vertex& l, const Vertex& r);

	/**
	 * return true if l.y is smaller than or equal to u.y
	 *
	 * @param l vertex supposedly with smaller y
	 * @param u vertex supposedly with larger y
	 */
	bool LowerVertex(const Vertex& l, const Vertex& u);
}
