#include "Container.h"
#include <algorithm>
#include <execution>

Container::Container(const Size& containerSize) :
	_m_size(containerSize)
{
	// center of the container is placed in the origin
	auto wdiv2 = _m_size.width() >> 1;
	auto hdiv2 = _m_size.height() >> 1;
	std::vector<vec2i> initPoints = { {0 - wdiv2,0 - hdiv2},
		{containerSize.width() - wdiv2,0 - hdiv2},
		{containerSize.width() - wdiv2, containerSize.height() - hdiv2},
		{0 - wdiv2,containerSize.height() - hdiv2} };
	_m_emptyAreas.emplace_back(initPoints);
}

bool Container::tryToInsertRect(const Size& rectSize)
{
	for (size_t areaIndex = 0; areaIndex < _m_emptyAreas.size(); areaIndex++)
	{
		auto edges = _m_emptyAreas[areaIndex].edges();
		std::vector<size_t> pushOutIndexes;
		vec2i curNormal = edges.front().normal();
		vec2i nextNormal;
		// Find concave corners. Only in them we will try to place rectangles
		for (size_t cornerIndex = 0; cornerIndex < edges.size(); cornerIndex++)
		{
			auto nextIndex = (cornerIndex + 1) % edges.size();
			nextNormal = edges[nextIndex].normal();

			if (curNormal.X() < 0 && nextNormal.Y() < 0 ||
				curNormal.Y() < 0 && nextNormal.X() > 0 ||
				curNormal.X() > 0 && nextNormal.Y() > 0 ||
				curNormal.Y() > 0 && nextNormal.X() < 0) {
				pushOutIndexes.push_back(nextIndex);
			}

			curNormal = nextNormal;
		}

		auto corners = _m_emptyAreas[areaIndex].nodes();
		// sort the corners by distance from the origin
		std::sort(std::execution::seq, pushOutIndexes.begin(), pushOutIndexes.end(), [&corners](size_t a, size_t b) {
			return vec2(corners[a].X(), corners[a].Y()).len() > vec2(corners[b].X(), corners[b].Y()).len();
			});

		auto wdiv2 = _m_size.width() >> 1;
		auto hdiv2 = _m_size.height() >> 1;
		/// <summary>
		/// Trying to insert a new rectangle in the free area.
		/// </summary>
		/// <param name="origin">where</param>
		/// <param name="alongVec">max side direction</param>
		/// <param name="tangentVec">min side direction</param>
		/// <returns>success</returns>
		auto addAxesDefinedRect = [this, &areaIndex, &rectSize, &wdiv2, &hdiv2](const vec2i& origin, const vec2i& alongVec, const vec2i& tangentVec)->bool {
			Recti testedRect((origin + alongVec * rectSize.maxSide()), (origin + tangentVec * rectSize.minSide()));
			auto result = _m_emptyAreas[areaIndex].tryToCut(testedRect);

			if (result.m_succesfull) {
				// back to original coordinate system
				testedRect.move(vec2i(wdiv2, hdiv2));
				_m_boxes.push_back(testedRect);
			}

			for (auto& newContour : result.m_newContours)
			{
				_m_emptyAreas.push_back(newContour);
			}

			return result.m_succesfull;
		};

		// first try to place the rectangle in the farthest corner
		bool result = false;
		for (auto& cornerIndex : pushOutIndexes)
		{
			auto prevIndex = cornerIndex == 0 ? corners.size() - 1 : cornerIndex - 1;
			auto nextIndex = (cornerIndex + 1) % corners.size();

			auto toCenteri = corners[cornerIndex] * -1;
			auto toCenterf = vec2(toCenteri.X(), toCenteri.Y()).normilized();
			auto toPrevi = corners[prevIndex] - corners[cornerIndex];
			auto toPrevf = vec2(toPrevi.X(), toPrevi.Y()).normilized();
			toPrevi.setX(static_cast<int>(toPrevf.X())); toPrevi.setY(static_cast<int>(toPrevf.Y()));
			auto toNexti = corners[nextIndex] - corners[cornerIndex];
			auto toNextf = vec2(toNexti.X(), toNexti.Y()).normilized();
			toNexti.setX(static_cast<int>(toNextf.X())); toNexti.setY(static_cast<int>(toNextf.Y()));
			vec2i along = toNexti;
			vec2i tangent = toPrevi;
			if (vec2::dotProduct(toCenterf, toPrevf) > vec2::dotProduct(toCenterf, toNextf)) {
				along = toPrevi;
				tangent = toNexti;

			}

			if (!addAxesDefinedRect(corners[cornerIndex], along, tangent)) {
				if (addAxesDefinedRect(corners[cornerIndex], tangent, along)) {
					return true;
				}
			}
			else {
				return true;
			}
		}
	}

	return false;
}
