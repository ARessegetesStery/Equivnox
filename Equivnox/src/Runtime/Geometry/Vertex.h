#pragma once

#include <CoreMinimal.h>

#include "MeshTransform.h"

namespace EQX 
{
	/**    
	 * Axes, conforming to right-hand system
	 * 
	 *     ^ 
	 *     | y
	 *     |
	 *     | /
	 *     |/
	 * ----¡¤-----------> x
	 *    /|
	 * z / |
	 * 
	 */

	class Vertex
	{
	public: 
		Vector4 pos;
		Vector3 normal;		// Default set to be UNIT_Z
		Vector2 uv;
		Vector3 color;		// Default set to be white; each comp 0~255

	public:
		Vertex(Vector4 pos = Vector4::Zero, 
			Vector3 normal = Vector3::UnitZ, Vector2 uv = Vector2::Zero);
		Vertex(Vector3 pos, Vector3 normal = Vector3::UnitZ, Vector2 uv = Vector2::Zero);
		Vertex(Vector2 pos,	Vector3 normal = Vector3::UnitZ, Vector2 uv = Vector2::Zero);

		void MatTransform(const Mat4& projection);
		void MeshTransform(const MeshTransform& trans);
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

	// General operator<(s) defined for Vertex
	bool LefterLowerBehindVertex(const Vertex& l, const Vertex& r);
}
