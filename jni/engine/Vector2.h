/*
 * Vector2.h
 *
 *  Шаблон класса для работы с двухмерными векторами.
 *
 *  Дата создания: 12.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_

#include "Utils.h"

template <typename NumType>
class Vector2
{
	public:
		// тип значений
		typedef NumType ValueType;

		NumType x, y;

		Vector2() : x(0), y(0) {};
		Vector2(NumType x, NumType y) : x(x), y(y) {};

		// длина вектора
		NumType Length() const
		{
			return sqrt(x * x + y * y);
		}

		// если вектор равен другому
		bool operator== (const Vector2& v) const
		{
			return (x == v.x && y == v.y);
		}

		// если вектор не равен другому
		bool operator!= (const Vector2& v) const
		{
			return (x != v.x || y != v.y);
		}

		// сложение векторов
		Vector2 operator+ (const Vector2& v) const
		{
			return Vector2(x + v.x, y + v.y);
		}

		// вычитание векторов
		Vector2 operator- (const Vector2& v) const
		{
			return Vector2(x - v.x, y - v.y);
		}

		// прибавить вектор
		void operator+= (const Vector2& v)
		{
			x += v.x;
			y += v.y;
		}

		// вычесть вектор
		void operator-= (const Vector2& v)
		{
			x -= v.x;
			y -= v.y;
		}

		// умножить на вектор
		void operator*= (const Vector2& v)
		{
			x *= v.x;
			y *= v.y;
		}

		// разделить на вектор
		void operator/= (const Vector2& v)
		{
			x /= v.x;
			y /= v.y;
		}

		// Умножение на скаляр
		Vector2 operator* (const NumType value) const
		{
			return Vector2(x * value, y * value);
		}

		// Умножение на вектор
		Vector2 operator* (const Vector2& v) const
		{
			return Vector2(x * v.x, y * v.y);
		}

		// Деление на скаляр
		Vector2 operator/ (const NumType value) const
		{
			return Vector2(x / value, y / value);
		}

		// Деление на вектор
		Vector2 operator/ (const Vector2& v) const
		{
			return Vector2(x / v.x, y / v.y);
		}

		// Нормализация
		Vector2 Normalize() const
		{
			NumType leni = 1.0 / sqrt(x * x + y * y);
			return Vector2(x * leni, y * leni);
		}

		// отрицательные значения вектора
		Vector2 Negative() const
		{
			return Vector2(-x, -y);
		}

		// Скалярное произведение векторов
		NumType Dot(const Vector2& v) const
		{
			return x * v.x + y * v.y;
		}

		// перпендикуляр
		Vector2 Perpendicular() const
		{
			return Vector2(-y, x);
		}

		// Векторное произведение
		NumType Cross(const Vector2& v) const
		{
			return x * v.y - y * v.x;
		}


		// Повернуть вектор с помощью вектора направления (cos, sin)
		Vector2 Rotate(const Vector2& rv) const
		{
			return Vector2(x * rv.x - y * rv.y, x * rv.y + y * rv.x);
		}

		// Повернуть вектор с помощью (cos, sin) угла
		Vector2 Rotate(const NumType cosA, const NumType sinA) const
		{
			return Vector2(x * cosA - y * sinA, x * sinA + y * cosA);
		}

		// Повернуть на угол в радианах
		Vector2 Rotate(const NumType angle) const
		{
			NumType cosA = cos(angle);
			NumType sinA = sin(angle);
			return Vector2(x * cosA - y * sinA, x * sinA + y * cosA);
		}

};

// Посчитать вектор направления. Угол в радианах
template <typename NumType>
static inline Vector2<NumType> DirectionVector(NumType angle)
{
	return Vector2<NumType>(cos(angle), sin(angle));
}

// Посчитать вектор направления. Угол в градусах
template <typename NumType>
static inline Vector2<NumType> DirectionVectorD(NumType degreeAngle)
{
	NumType angle = degreeAngle * RADIAN;
	angle -= M_PI / 2.0;
	return Vector2<NumType>(cos(angle), sin(angle));
}

// Получить угол наклона в градусах с помощью вектора расстояния от точки до точки
template <typename NumType>
static inline NumType DegreeAngle(const Vector2<NumType>& v)
{
	NumType angle = (atan2(v.y, v.x) + (M_PI / 2.0)) * DEGREE;

	if (angle < 0)
		angle += 360;

	return angle;
}

// Получить угол наклона в градусах с помощью вектора расстояния от точки до точки
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
