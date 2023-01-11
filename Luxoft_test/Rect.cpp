#include <limits>
#include "Rect.h"

Rect::Rect(const vec2& downLeft, const Size& size) :
	_downLeft(downLeft)
{
	extend(_downLeft + vec2(size.width(), size.height()));
}

Rect::Rect(const vec2& p1, const vec2& p2)
{
	_downLeft.setX(std::numeric_limits<double>::max());
	_downLeft.setY(std::numeric_limits<double>::max());

	_upRight.setX(-std::numeric_limits<double>::max());
	_upRight.setY(-std::numeric_limits<double>::max());

	extend(p1);
	extend(p2);
}

void Rect::extend(const vec2& newPoint)
{
	_downLeft.setX(std::min(newPoint.X(), _downLeft.X()));
	_downLeft.setY(std::min(newPoint.Y(), _downLeft.Y()));
	_upRight.setX(std::max(newPoint.X(), _upRight.X()));
	_upRight.setY(std::max(newPoint.Y(), _upRight.Y()));
}

//void Rect::rotate(double angle)
//{
//	_currentAngle += angle;
//	calculateMatrix();
//}
//
//void Rect::setRotation(double angle)
//{
//	_currentAngle = angle;
//	calculateMatrix();
//}

void Rect::move(const vec2& delta)
{
	_downLeft = _downLeft + delta;
	_upRight = _upRight + delta;
}

std::vector<vec2> Rect::corners() const
{
	std::vector<vec2> result;
	result.push_back(_downLeft);
	result.push_back(vec2(_upRight.X(), _downLeft.Y()));
	result.push_back(_upRight);
	result.push_back(vec2(_downLeft.X(), _upRight.Y()));

	/*constexpr double eps5 = 0.00001;
	if (abs(_currentAngle) > eps5) {
		vec2 center(_downLeft + vec2(_size.width() / 2., _size.height() / 2.));
		auto mapToCenter = [&center](const vec2& p) {
			return p - center;
		};
		auto mapToOrigin = [&center](const vec2& p) {
			return p + center;
		};

		vec2 srcPoint, dstPoint;
		for (size_t i = 0; i < result.size(); i++)
		{
			srcPoint = mapToCenter(result[i]);
			dstPoint.setX(srcPoint.X() * _matrix[0] + srcPoint.Y() * _matrix[1]);
			dstPoint.setY(srcPoint.X() * _matrix[2] + srcPoint.Y() * _matrix[3]);
			result[i] = mapToOrigin(dstPoint);
		}
	}*/

	return result;
}
