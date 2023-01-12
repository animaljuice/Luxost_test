#pragma once

#include <iostream>

class Size {
	int _w, _h;

public:
	Size(int w = 0, int h = 0) :
		_w(w), _h(h)
	{

	}

	auto width() const { return _w; }
	auto height() const { return _h; }
	void setWidth(int w) { _w = w; }
	void setHeight(int h) { _h = h; }
	auto maxSide() const { return std::max(_w, _h); }
	auto minSide() const { return std::min(_w, _h); }

	bool operator<(const Size& other) const {
		return std::max(_w, _h) < std::max(other._w, other._h);
	}

	friend std::ostream& operator<<(std::ostream& os, const Size& sz);
};

inline std::ostream& operator<<(std::ostream& os, const Size& sz) {
	os << sz._w << ',' << sz._h;
	return os;
}