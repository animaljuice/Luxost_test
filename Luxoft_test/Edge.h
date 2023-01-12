#pragma once

#include "vec2.h"

enum class Orientation {
	HORIZONTAL,
	VERTICAL
};

struct Edge
{
	int m_from, m_to;
	Orientation m_orientation;
	int m_position;

	bool operator<(const Edge& other) const {
		return m_position < other.m_position;
	}

	vec2i normal() const {
		vec2i result;
		if (m_orientation == Orientation::HORIZONTAL) {
			if (m_to > m_from) {
				result.setY(1);
			}
			else {
				result.setY(-1);
			}
		}
		else {
			if (m_to > m_from) {
				result.setX(1);
			}
			else {
				result.setX(-1);
			}
		}

		return result;
	}
};

