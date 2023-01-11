#include "Contour.h"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Point_2 = Kernel::Point_2;
using Polygon_2 = CGAL::Polygon_2<Kernel>;
using Polygon_with_holes_2 = CGAL::Polygon_with_holes_2<Kernel>;

std::vector<vec2> polyVertices(const Polygon_2& poly) {
	auto& nativeVertices = poly;
	std::vector<vec2> result(nativeVertices.size());
	for (size_t vertexIndex = 0; vertexIndex < nativeVertices.size(); vertexIndex++)
	{
		result[vertexIndex].setX(CGAL::to_double(nativeVertices[vertexIndex].x()));
		result[vertexIndex].setY(CGAL::to_double(nativeVertices[vertexIndex].y()));
	}
	return result;
}

class ContourImpl {
public:
	Polygon_with_holes_2 m_polygon;
};

Contour::Contour():
	_m_pImpl(new ContourImpl)
{
}

Contour::Contour(const std::vector<vec2>& points):
	_m_pImpl(new ContourImpl)
{
	Polygon_2 basePolygon;
	for (auto it = points.begin(); it != points.end(); it++)
	{
		basePolygon.push_back(Point_2(it->X(), it->Y()));
	}

	_m_pImpl->m_polygon = Polygon_with_holes_2(basePolygon);
}

Contour::Contour(const Contour& other):
	_m_pImpl(new ContourImpl)
{
	_m_pImpl->m_polygon = _m_pImpl->m_polygon;
}

Contour& Contour::operator=(const Contour& other)
{
	if (this != &other) {
		_m_pImpl->m_polygon = other._m_pImpl->m_polygon;
	}

	return *this;
}

Contour::~Contour()
{
	delete _m_pImpl;
}

std::vector<vec2> Contour::vertices() const
{
	return polyVertices(_m_pImpl->m_polygon.outer_boundary());
}

Contour::CutResult Contour::tryToCut(const Rect& testedRect)
{
	CutResult result;

	auto rectPoints = testedRect.corners();
	std::unique_ptr<Polygon_2> rectPoly = std::make_unique<Polygon_2>(); 
	for (auto it = rectPoints.begin(); it != rectPoints.end(); it++)
	{
		rectPoly->push_back(Point_2(it->X(), it->Y()));
	}

	std::list<Polygon_with_holes_2> intR;
	CGAL::intersection(_m_pImpl->m_polygon, *rectPoly, std::back_inserter(intR));
	if (intR.size() > 0 && !intR.begin()->has_holes()) {
		if (CGAL::to_double((*intR.begin()).outer_boundary().area()) == CGAL::to_double(rectPoly->area())) {
			intR.clear();
			CGAL::difference(_m_pImpl->m_polygon, *rectPoly, std::back_inserter(intR));
			bool firstPolygon = true;
			for (auto& resultPolyWithHole : intR) {
				result.m_succesfull = true;
				if (firstPolygon) {
					firstPolygon = false;
					_m_pImpl->m_polygon = Polygon_with_holes_2(resultPolyWithHole);
				}
				else {
					Contour newContour;
					newContour._m_pImpl->m_polygon = resultPolyWithHole;
					result.m_newContours.push_back(newContour);
				}
			}
		}
	}

	return result;
}
