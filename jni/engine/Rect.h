/*
 * Rect.h
 *
 *  ������ ������ ��� ������ � �������������� ���������.
 *
 *  ���� ��������: 14.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef RECT_H_
#define RECT_H_

#include "Utils.h"

template <typename NumType>
class Rect
{
	public:
		// ���������� ����� ������� ����� �� ��� X
		NumType left;
		// ���������� ����� ������� ����� �� ��� Y
		NumType top;
		// ���������� ������ ������ ����� �� ��� X
		NumType right;
		// ���������� ������ ������ ����� �� ��� Y
		NumType bottom;

		Rect() : left(0), top(0), right(0), bottom(0) {};

		Rect(NumType left, NumType top, NumType right, NumType bottom) :
			left(left), top(top), right(right), bottom(bottom) {};

		// ��������� �� ����� (x, y) � ��������������
		bool Contains(NumType x, NumType y) const
		{
			return (x >= left && x < right &&
					y >= top && y < bottom);
		}

		// ������������ �� ��� ��������������
		bool Intersects(const Rect& r2) const
		{
			return (r2.left <= right && left <= r2.right &&
				 r2.top <= bottom && top <= r2.bottom);
		}

		// ������������ �� ��� ��������������.
		// ���������� �������������, ���������� � ���������� �����������.
		bool GetIntersectionRect(const Rect& r2, Rect& dest) const
		{
			bool i = Intersects(r2);

			if (i)
			{
				dest.left = Max<NumType>(left, r2.left);
				dest.right = Min<NumType>(right, r2.right);
				dest.top = Max<NumType>(top, r2.top);
				dest.bottom = Min<NumType>(bottom, r2.bottom);
			}
			return i;
		}
};

// ������� ������������ ������������� � ������������ (x, y) ������������ ������
template <typename NumType>
Rect<NumType> RectBounds(NumType x, NumType y, NumType width, NumType height)
{
	return Rect<NumType>(x, y, x + width, y + height);
}

#endif /* RECT_H_ */
