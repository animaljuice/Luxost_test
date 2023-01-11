#pragma once

#include <cmath>
#include <iostream>

class vec2 {
	double _x, _y;
public:
	vec2(double x = 0., double y = 0.) :
		_x(x), _y(y)
	{

	}

	auto X() const { return _x; }
	auto Y() const { return _y; }

	void setX(double x) { _x = x; }
	void setY(double y) { _y = y; }

	void round() {
		_x = std::round(_x);
		_y = std::round(_y);
	}

	vec2 rounded() const {
		auto result = *this;
		result.round();
		return result;
	}

	vec2 operator+(const vec2& other) const {
		return { other._x + _x, other._y + _y };
	}

	vec2 operator-(const vec2& other) const {
		return { _x - other._x, _y - other._y };
	}

	auto operator/(double den) const {
		return vec2(_x / den, _y / den);
	}

	auto operator*(double num) const {
		return vec2(_x * num, _y * num);
	}

	auto len() const {
		return sqrt(_x * _x + _y * _y);
	}

	auto normilized() const {
		return (*this) / len();
	}

	static inline double dotProduct(const vec2& p1, const vec2& p2) {
		return p1._x * p2._x + p1._y * p2._y;
	}

	friend std::ostream& operator<<(std::ostream& os, const vec2& p);
};

inline std::ostream& operator<<(std::ostream& os, const vec2& p) {
	os << p.X() << ',' << p.Y();
	return os;
}