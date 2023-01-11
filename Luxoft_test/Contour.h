#pragma once

#include <vector>
#include "Rect.h"
#include "point.h"

class ContourImpl;
class Contour
{
	ContourImpl* _m_pImpl;

private:
	Contour();

public:

	struct CutResult {
		bool m_succesfull = false;
		std::vector<Contour> m_newContours;
	};

	Contour(const std::vector<vec2>& points);
	Contour(const Contour& other);
	Contour& operator=(const Contour& other);
	virtual ~Contour();

	std::vector<vec2> vertices() const;
	CutResult tryToCut(const Rect& testedRect);

};

