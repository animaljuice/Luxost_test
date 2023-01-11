#pragma once

#include <vector>
#include "point.h"

class AAPolygon
{
	vec2 _m_start;
	std::vector<double> _m_edges;

public:
	AAPolygon(const vec2& startPoint, const std::vector<double>& edges);
};

