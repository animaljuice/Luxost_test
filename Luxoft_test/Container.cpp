#include "Container.h"
#include <algorithm>
#include <execution>

Container::Container(const Size& containerSize) :
	_m_size(containerSize)
{
	auto wdiv2 = _m_size.width() / 2.;
	auto hdiv2 = _m_size.height() / 2.;
	std::vector<vec2> initPoints = { {-wdiv2,-hdiv2}, {wdiv2,-hdiv2}, {wdiv2,hdiv2}, {-wdiv2,hdiv2} };
	_m_emptyAreas.emplace_back(initPoints);
}

bool Container::tryToAddRect(const Size& rectSize)
{
	auto wdiv2 = _m_size.width() / 2.;
	auto hdiv2 = _m_size.height() / 2.;

	for (size_t areaIndex = 0; areaIndex < _m_emptyAreas.size(); areaIndex++)
	{
		auto corners = _m_emptyAreas[areaIndex].vertices();
		std::vector<size_t> pushOutIndexes;
		for (size_t cornerIndex = 0; cornerIndex < corners.size(); cornerIndex++)
		{
			auto prevIndex = cornerIndex == 0 ? corners.size() - 1 : cornerIndex - 1;
			auto nextIndex = (cornerIndex + 1) % corners.size();
			if ((corners[nextIndex].X() - corners[prevIndex].X()) *
				(corners[cornerIndex].Y() - corners[prevIndex].Y()) -
				(corners[nextIndex].Y() - corners[prevIndex].Y()) *
				(corners[cornerIndex].X() - corners[prevIndex].X()) < 0) {
				pushOutIndexes.push_back(cornerIndex);
			}
		}

		std::sort(std::execution::seq, pushOutIndexes.begin(), pushOutIndexes.end(), [&corners](size_t a, size_t b) {
			return corners[a].len() > corners[b].len();
			});

		auto addAxesDefinedRect = [this, &areaIndex, &rectSize, &wdiv2, &hdiv2](const vec2& origin, const vec2& alongVec, const vec2& tangentVec)->bool {
			Rect testedRect((origin + alongVec * rectSize.maxSide()).rounded(),
				(origin + tangentVec * rectSize.minSide()).rounded());
			auto result = _m_emptyAreas[areaIndex].tryToCut(testedRect);

			if (result.m_succesfull) {

				testedRect.move(vec2(wdiv2, hdiv2));
				_m_boxes.push_back(testedRect);
			}

			for (auto& newContour : result.m_newContours)
			{
				_m_emptyAreas.push_back(newContour);
			}

			return result.m_succesfull;
		};

		bool result = false;
		for (auto& cornerIndex : pushOutIndexes)
		{
			auto prevIndex = cornerIndex == 0 ? corners.size() - 1 : cornerIndex - 1;
			auto nextIndex = (cornerIndex + 1) % corners.size();

			auto toCenter = (corners[cornerIndex] * -1.).normilized();
			auto toPrev = (corners[prevIndex] - corners[cornerIndex]).normilized();
			auto toNext = (corners[nextIndex] - corners[cornerIndex]).normilized();
			vec2 along = toNext;
			vec2 tangent = toPrev;
			if (vec2::dotProduct(toCenter, toPrev) > vec2::dotProduct(toCenter, toNext)) {
				along = toPrev;
				tangent = toNext;
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
