#pragma once

#include <vector>
#include <set>
#include "vec2.h"
#include "Edge.h"
#include "Recti.h"

/// <summary>
/// Axis aligned polygon
/// </summary>
class AAPolygon
{
	std::vector<vec2i> _m_nodes;
	std::vector<Edge> _m_cache;
	/// <summary>
	/// test if the rect inside the polygon or not
	/// </summary>
	/// <param name="testedRect"></param>
	/// <returns></returns>
	bool insideTest(const Recti& testedRect);

	AAPolygon(){}

public:
	/// <summary>
	/// result of cutting polygon by rectangle
	/// </summary>
	struct CutResult {
		/// <summary>
		/// success
		/// </summary>
		bool m_succesfull = true;
		/// <summary>
		/// new polygons. Rectangle can divide polygon to several new
		/// </summary>
		std::vector<AAPolygon> m_newContours;
	};

	AAPolygon(const std::vector<vec2i>& nodes);
	std::vector<Edge> edges();
	std::multiset<Edge> edges(Orientation o);
	std::vector<vec2i> nodes() const {
		return _m_nodes;
	}

	/// <summary>
	/// reduce area of bolygon due clipping it by rectangle.
	/// for success rectangle can't intersect any polygon's edge
	/// </summary>
	/// <param name="testedRect"></param>
	/// <returns></returns>
	CutResult tryToCut(const Recti& testedRect);
};