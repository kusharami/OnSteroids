/*
 * Vector2.h
 *
 *  ������ ������ ��� ������ � ����������� ���������.
 *
 *  ���� ��������: 12.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_

#include "Utils.h"

template <typename NumType>
class Vector2
{
	public:
		// ��� ��������
		typedef NumType ValueType;

		NumType x, y;

		Vector2() : x(0), y(0) {};
		Vector2(NumType x, NumType y) : x(x), y(y) {};

		// ����� �������
		NumType Length() const
		{
			return sqrt(x * x + y * y);
		}

		// ���� ������ ����� �������
		bool operator== (const Vector2& v) const
		{
			return (x == v.x && y == v.y);
		}

		// ���� ������ �� ����� �������
		bool operator!= (const Vector2& v) const
		{
			return (x != v.x || y != v.y);
		}

		// �������� ��������
		Vector2 operator+ (const Vector2& v) const
		{
			return Vector2(x + v.x, y + v.y);
		}

		// ��������� ��������
		Vector2 operator- (const Vector2& v) const
		{
			return Vector2(x - v.x, y - v.y);
		}

		// ��������� ������
		void operator+= (const Vector2& v)
		{
			x += v.x;
			y += v.y;
		}

		// ������� ������
		void operator-= (const Vector2& v)
		{
			x -= v.x;
			y -= v.y;
		}

		// �������� �� ������
		void operator*= (const Vector2& v)
		{
			x *= v.x;
			y *= v.y;
		}

		// ��������� �� ������
		void operator/= (const Vector2& v)
		{
			x /= v.x;
			y /= v.y;
		}

		// ��������� �� ������
		Vector2 operator* (const NumType value) const
		{
			return Vector2(x * value, y * value);
		}

		// ��������� �� ������
		Vector2 operator* (const Vector2& v) const
		{
			return Vector2(x * v.x, y * v.y);
		}

		// ������� �� ������
		Vector2 operator/ (const NumType value) const
		{
			return Vector2(x / value, y / value);
		}

		// ������� �� ������
		Vector2 operator/ (const Vector2& v) const
		{
			return Vector2(x / v.x, y / v.y);
		}

		// ������������
		Vector2 Normalize() const
		{
			NumType leni = 1.0 / sqrt(x * x + y * y);
			return Vector2(x * leni, y * leni);
		}

		// ������������� �������� �������
		Vector2 Negative() const
		{
			return Vector2(-x, -y);
		}

		// ��������� ������������ ��������
		NumType Dot(const Vector2& v) const
		{
			return x * v.x + y * v.y;
		}

		// �������������
		Vector2 Perpendicular() const
		{
			return Vector2(-y, x);
		}

		// ��������� ������������
		NumType Cross(const Vector2& v) const
		{
			return x * v.y - y * v.x;
		}


		// ��������� ������ � ������� ������� ����������� (cos, sin)
		Vector2 Rotate(const Vector2& rv) const
		{
			return Vector2(x * rv.x - y * rv.y, x * rv.y + y * rv.x);
		}

		// ��������� ������ � ������� (cos, sin) ����
		Vector2 Rotate(const NumType cosA, const NumType sinA) const
		{
			return Vector2(x * cosA - y * sinA, x * sinA + y * cosA);
		}

		// ��������� �� ���� � ��������
		Vector2 Rotate(const NumType angle) const
		{
			NumType cosA = cos(angle);
			NumType sinA = sin(angle);
			return Vector2(x * cosA - y * sinA, x * sinA + y * cosA);
		}

};

// ��������� ������ �����������. ���� � ��������
template <typename NumType>
static inline Vector2<NumType> DirectionVector(NumType angle)
{
	return Vector2<NumType>(cos(angle), sin(angle));
}

// ��������� ������ �����������. ���� � ��������
template <typename NumType>
static inline Vector2<NumType> DirectionVectorD(NumType degreeAngle)
{
	NumType angle = degreeAngle * RADIAN;
	angle -= M_PI / 2.0;
	return Vector2<NumType>(cos(angle), sin(angle));
}

// �������� ���� ������� � �������� � ������� ������� ���������� �� ����� �� �����
template <typename NumType>
static inline NumType DegreeAngle(const Vector2<NumType>& v)
{
	NumType angle = (atan2(v.y, v.x) + (M_PI / 2.0)) * DEGREE;

	if (angle < 0)
		angle += 360;

	return angle;
}

// �������� ���� ������� � �������� � ������� ������� ���������� �� ����� �� �����
template <typename NumType>
static inline NumType DegreeAngle(NumType diffX, NumType diffY)
{
	NumType angle = (atan2(diffY, diffX) + (M_PI / 2.0)) * DEGREE;

	if (angle < 0)
		angle += 360;

	return angle;
}


typedef Vector2<float> Vector2f;

#endif /* VECTOR2_H_ */
