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
	 * ----��-----------> x
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

}
