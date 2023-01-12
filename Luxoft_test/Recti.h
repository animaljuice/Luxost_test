#pragma once

#include <vector>
#include "size.h"
#include "vec2.h"

class Recti
{
	vec2i _m_downLeft;
	vec2i _m_upRight;

public:
	Recti(const vec2i& downLeft, const Size& size);
	Recti(const vec2i& p1, const vec2i& p2);

	void extend(const vec2i& newPoint);
	void move(const vec2i& delta);

	std::vector<vec2i> corners() const;
	int left() const { return _m_downLeft.X(); }
	int right() const { return _m_upRight.X(); }
	int down() const { return _m_downLeft.Y(); }
	int up() const { return _m_upRight.Y(); }

	friend std::ostream& operator<<(std::ostream& os, const Recti& r);
};

inline std::ostream& operator<<(std::ostream& os, const Recti& r) {
	auto contour = r.corners();
	for (size_t pointIndex = 0; pointIndex < contour.size(); pointIndex++) {
		os << contour[pointIndex];
		if (pointIndex < contour.size() - 1) {
			os << ',';
		}
	}
	return os;
}

