#include "Recti.h"

Recti::Recti(const vec2i& downLeft, const Size& size):
	_m_downLeft(downLeft)
{
	extend(_m_downLeft + vec2i(size.width(), size.height()));
}

Recti::Recti(const vec2i& p1, const vec2i& p2)
{
	_m_downLeft.setX(std::numeric_limits<int>::max());
	_m_downLeft.setY(std::numeric_limits<int>::max());

	_m_upRight.setX(-std::numeric_limits<int>::max());
	_m_upRight.setY(-std::numeric_limits<int>::max());

	extend(p1);
	extend(p2);
}

void Recti::extend(const vec2i& newPoint)
{
	_m_downLeft.setX(std::min(newPoint.X(), _m_downLeft.X()));
	_m_downLeft.setY(std::min(newPoint.Y(), _m_downLeft.Y()));
	_m_upRight.setX(std::max(newPoint.X(), _m_upRight.X()));
	_m_upRight.setY(std::max(newPoint.Y(), _m_upRight.Y()));
}

void Recti::move(const vec2i& delta)
{
	_m_downLeft = _m_downLeft + delta;
	_m_upRight = _m_upRight + delta;
}

std::vector<vec2i> Recti::corners() const
{
	std::vector<vec2i> result;
	result.push_back(_m_downLeft);
	result.push_back(vec2i(_m_upRight.X(), _m_downLeft.Y()));
	result.push_back(_m_upRight);
	result.push_back(vec2i(_m_downLeft.X(), _m_upRight.Y()));

	return result;
}
