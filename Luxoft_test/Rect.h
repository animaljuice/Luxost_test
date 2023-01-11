#pragma once

#include <vector>
#include "structs.h"
#include "point.h"

class Rect {
	vec2 _downLeft;
	vec2 _upRight;
	/*double _matrix[4];
	double _currentAngle;*/

	/*void calculateMatrix() {
		_matrix[0] = cos(_currentAngle);
		_matrix[1] = -sin(_currentAngle);
		_matrix[2] = sin(_currentAngle);
		_matrix[3] = cos(_currentAngle);
	}*/

public:
	Rect(const vec2& downLeft, const Size& size);
	Rect(const vec2& p1, const vec2& p2);

	void extend(const vec2& newPoint);
	/*void rotate(double angle);
	void setRotation(double angle);*/
	void move(const vec2& delta);

	std::vector<vec2> corners() const;
	double left() const{
		return _downLeft.X();
	}
	double right() const {
		return _upRight.X();
	}
	double down() const {
		return _downLeft.Y();
	}
	double up() const {
		return _upRight.Y();
	}

	friend std::ostream& operator<<(std::ostream& os, const Rect& r);
};

inline std::ostream& operator<<(std::ostream& os, const Rect& r) {
	auto contour = r.corners();
	for (size_t pointIndex = 0; pointIndex < contour.size(); pointIndex++) {
		os << contour[pointIndex];
		if (pointIndex < contour.size() - 1) {
			os << ',';
		}
	}
	return os;
}
