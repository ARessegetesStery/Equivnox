#pragma once

#include <CoreMinimal.h>

#include "Math/MathHeader.h"
#include "Runtime/Geometry/Geometry.h"

// To see the logs, comment the line below. 
// Note that logging significantly slows the rendering process
#undef EQX_PRINT_TRIG_CLIPPING

namespace EQX {

	void floorVertexXYPos(Vertex& v);

	/**
	 * Apply Clipping in Clip Space
	 * The clipped triangles will be pushed to the back of {faces}
	 *
	 * @param inFace - the face to be evaluated
	 * @param faces - [OUT]the faces that need to be rendered
	 */
	void FrustumClipping(const Face& inFace, EQX_OUT Mesh& clippedMesh);

	/**
	 * @param intersections - {UnitBoxIntersection} in {FrustumClipping}
	 * @param l - [OUT]expression for corner edge
	 * @returns the expression for the edge to be tested
	 */
	XBool CornerEdge(const std::vector<Vertex>& intersections, EQX_OUT std::vector<Line>& l);

	/**
	 * Requires that all the vertices are on the same plane
	 * Does not guarantee the sequence of elements in {vertices}
	 * 
	 * @param vertices - the vertices of the control point
	 * @param trigs - [OUT] the resulting parsed triangles
	 */
	void TriangularizeHull(std::vector<Vertex>& vertices, EQX_OUT Mesh& clippedMesh);

}
