#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>

#include "AAPolygon.h"

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Point_2 = Kernel::Point_2;
using Polygon_2 = CGAL::Polygon_2<Kernel>;
using Polygon_with_holes_2 = CGAL::Polygon_with_holes_2<Kernel>;

std::vector<Polygon_2> disjoinNotSimplePolygon(const Polygon_2& notSimplePolygon) {
	std::vector<Polygon_2> result;
	if (notSimplePolygon.is_simple()) {
		result.push_back(notSimplePolygon);
	}
	else {
		auto makeNewPolygon = [&notSimplePolygon, &result](Polygon_2::Vertex_iterator start, Polygon_2::Vertex_iterator end) {
			Polygon_2 newPolygon;
			for (; true; start++) {
				if (start == notSimplePolygon.vertices_end()) {
					start = notSimplePolygon.vertices_begin();
				}

				newPolygon.push_back(*start);

				if (start == end) {
					newPolygon.erase(newPolygon.vertices_end() - 1);
					break;
				}
			}

			auto nextLevelPolygons = disjoinNotSimplePolygon(newPolygon);
			result.insert(result.end(), nextLevelPolygons.begin(), nextLevelPolygons.end());
		};

		for (auto firstVertexIt = notSimplePolygon.vertices_begin(); firstVertexIt != notSimplePolygon.vertices_end(); firstVertexIt++) {
			for (auto secondVertexIt = firstVertexIt + 1; secondVertexIt != notSimplePolygon.vertices_end(); secondVertexIt++) {
				if (*firstVertexIt == *secondVertexIt) {
					makeNewPolygon(firstVertexIt, secondVertexIt);
					makeNewPolygon(secondVertexIt, firstVertexIt);
					return result;
				}
			}
		}
	}

	return result;
}

bool intervalsIntersect(int f1, int t1, int f2, int t2) {
	return f1 < t2&& t1 > f2;
}

bool AAPolygon::insideTest(const Recti& testedRect)
{
	auto horizontalEdges = edges(Orientation::HORIZONTAL);
	for (auto edgeIt = horizontalEdges.begin(); edgeIt != horizontalEdges.end(); edgeIt++) {
		if ((*edgeIt).m_position > testedRect.down() && (*edgeIt).m_position < testedRect.up()) {
			if (intervalsIntersect(testedRect.left(), testedRect.right(),
				std::min((*edgeIt).m_from, (*edgeIt).m_to), std::max((*edgeIt).m_from, (*edgeIt).m_to))) {
				return false;
			}
		}
	}
	auto verticalEdges = edges(Orientation::VERTICAL);
	for (auto edgeIt = verticalEdges.begin(); edgeIt != verticalEdges.end(); edgeIt++) {
		if ((*edgeIt).m_position > testedRect.left() && (*edgeIt).m_position < testedRect.right()) {
			if (intervalsIntersect(testedRect.down(), testedRect.up(),
				std::min((*edgeIt).m_from, (*edgeIt).m_to), std::max((*edgeIt).m_from, (*edgeIt).m_to))) {
				return false;
			}
		}
	}

	return true;
}

AAPolygon::AAPolygon(const std::vector<vec2i>& nodes) :
	_m_nodes(nodes)
{
}

std::vector<Edge> AAPolygon::edges()
{
	if (_m_cache.empty()) {
		Edge currentEdge{};
		for (auto nodeIt = _m_nodes.begin(); nodeIt != _m_nodes.end(); nodeIt++) {
			auto nextNodeIt = nodeIt;
			nextNodeIt++;
			if (nextNodeIt == _m_nodes.end()) nextNodeIt = _m_nodes.begin();
			if ((*nodeIt).X() == (*nextNodeIt).X()) {
				currentEdge.m_orientation = Orientation::VERTICAL;
				currentEdge.m_from = (*nodeIt).Y();
				currentEdge.m_to = (*nextNodeIt).Y();
				currentEdge.m_position = (*nodeIt).X();
			}
			else {
				currentEdge.m_orientation = Orientation::HORIZONTAL;
				currentEdge.m_from = (*nodeIt).X();
				currentEdge.m_to = (*nextNodeIt).X();
				currentEdge.m_position = (*nodeIt).Y();
			}
			_m_cache.push_back(currentEdge);
		}
	}

	return _m_cache;
}

std::multiset<Edge> AAPolygon::edges(Orientation o)
{
	std::multiset<Edge> result;
	auto thisEdges = edges();
	for (size_t edgeIndex = 0; edgeIndex < thisEdges.size(); edgeIndex++) {
		if (thisEdges[edgeIndex].m_orientation == o) {
			result.insert(thisEdges[edgeIndex]);
		}
	}
	return result;
}

AAPolygon::CutResult AAPolygon::tryToCut(const Recti& testedRect)
{
	AAPolygon::CutResult result;
	if (!insideTest(testedRect)) {
		result.m_succesfull = false;
	}
	else {
		auto rectPoints = testedRect.corners();
		Polygon_2 rectPoly;
		for (auto it = rectPoints.begin(); it != rectPoints.end(); it++)
		{
			rectPoly.push_back(Point_2(it->X(), it->Y()));
		}

		Polygon_2 thisPoly;
		for (auto it = _m_nodes.begin(); it != _m_nodes.end(); it++)
		{
			thisPoly.push_back(Point_2(it->X(), it->Y()));
		}

		auto cgalPolyToVector = [](const Polygon_2& cgalPoly) {
			std::vector<vec2i> result;
			for (auto vertexIt = cgalPoly.vertices_begin(); vertexIt != cgalPoly.vertices_end(); vertexIt++) {
				result.push_back(vec2i(CGAL::to_double((*vertexIt).x()), CGAL::to_double((*vertexIt).y())));
			}
			return result;
		};

		std::list<Polygon_with_holes_2> intR;
		CGAL::difference(thisPoly, rectPoly, std::back_inserter(intR));

		bool firstPolygon = true;
		for (auto& resultPolyWithHole : intR) {
			if (!resultPolyWithHole.outer_boundary().is_simple()) {
				auto disjoinedParts = disjoinNotSimplePolygon(resultPolyWithHole.outer_boundary());
				resultPolyWithHole = Polygon_with_holes_2(disjoinedParts.front());
				for(auto polygonIt = disjoinedParts.begin() + 1; polygonIt != disjoinedParts.end(); ++polygonIt) {
					intR.emplace_back(*polygonIt);
				}
			}
			if (firstPolygon) {
				firstPolygon = false;
				_m_cache.clear();
				_m_nodes = cgalPolyToVector(resultPolyWithHole.outer_boundary());
			}
			else {
				AAPolygon newContour;
				newContour._m_nodes = cgalPolyToVector(resultPolyWithHole.outer_boundary());;
				result.m_newContours.push_back(newContour);
			}
		}
	}

	return result;
}
