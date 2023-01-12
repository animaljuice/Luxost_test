#pragma once

#include <vector>
#include <set>
#include "vec2.h"
#include "Edge.h"
#include "Recti.h"

class AAPolygon
{
	std::vector<vec2i> _m_nodes;
	std::vector<Edge> _m_cache;
	bool insideTest(const Recti& testedRect);

	AAPolygon(){}

public:
	struct CutResult {
		bool m_succesfull = true;
		std::vector<AAPolygon> m_newContours;
	};

	AAPolygon(const std::vector<vec2i>& nodes);
	std::vector<Edge> edges();
	std::multiset<Edge> edges(Orientation o);
	std::vector<vec2i> nodes() const {
		return _m_nodes;
	}

	CutResult tryToCut(const Recti& testedRect);
};

