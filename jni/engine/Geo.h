/*
 * Geo.h
 *
 *  Геометрические функции.
 *
 *  Дата создания: 04.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef GEO_H_
#define GEO_H_

#include <math.h>
#include "Vector2.h"
#include "Poly.h"
#include "Rect.h"

// Найти точку пересечения двух прямых
template <typename T>
bool LinesIntersect(const Vector2<T>& p1b, const Vector2<T>& p1e,
					const Vector2<T>& p2b, const Vector2<T>& p2e,
					Vector2<T>& point)
{
	float a1 = p1e.y - p1b.y;
	float b1 = p1b.x - p1e.x;

	float a2 = p2e.y - p2b.y;
	float b2 = p2b.x - p2e.x;

	float d = a1 * b2 - a2 * b1;

	if (d != 0)
	{
		float c1 = a1 * p1b.x + b1 * p1b.y;
		float c2 = a2 * p2b.x + b2 * p2b.y;
		point.x = (b2 * c1 - b1 * c2) / d;
		point.y = (a1 * c2 - a2 * c1) / d;
		return true;
	}
	return false;
}

// Узнать принадлежит ли точка заданному отрезку
template <typename T>
static inline bool LineContains(const Vector2<T>& pb, const Vector2<T>& pe, Vector2<T> point)
{
	// уравнение
	//px1+(1-p)x2=x
	//py1+(1-p)y2=y
	// 0<=p<=1
	T diff = pb.x - pe.x;
	if (diff != 0.0)
	{
		T p = (point.x - pe.x) / diff;

		return ((0.0 <= p <= 1.0) &&
				round((p * pb.y + (1 - p) * pe.y) * 100.0) == round(point.y * 100.0));
			// округлить до двух чисел после запятой
	}
	return false;
}

// Находится ли точка внутри произвольного многоугольника
template <typename NumType>
bool PolygonContains(const Poly<Vector2<NumType> >* poly, const Vector2<NumType>& point)
{
	// Проводим горизонтальный луч из точки point,
	// и проверяем сколько рёбер он пересекает.
	// Если кол-во нечётное, то точка находится внутри многоугольника
	int cnt = poly->vertexCount;
	int odd = 0;
	Vector2<NumType>* pe = poly->vertices;
	Vector2<NumType>* pb = &pe[cnt - 1];
	NumType k;
	while (cnt-- >= 0)
	{
		k = (pe->y - pb->y) / (pe->x - pb->x);

		if (((point.y - pb->y - k * pb->x) / k) >= point.x)
			odd ^= 1;

		pb++;
		pe++;
	}
	return odd;
}

// Пересекаются ли многоугольники
template <typename NumType>
bool PolygonsIntersect(const Poly<Vector2<NumType> >* p1, const Poly<Vector2<NumType> >* p2)
{
	const Vector2<NumType>* v1 = p1->vertices;
	const Vector2<NumType>* v2 = p2->vertices;

	// узнаём принадлежит ли прямоугольнику p2 одна из точек прямоугольника p1
	for (int i = p1->vertexCount; --i >= 0; )
		if (PolygonContains<NumType>(p2, v1[i]))
			return true;

	// узнаём принадлежит ли прямоугольнику p1 одна из точек прямоугольника p2
	for (int i = p2->vertexCount; --i >= 0; )
		if (PolygonContains<NumType>(p1, v2[i]))
			return true;


	// если точек не нашли, ищем пересекающиеся рёбра
	int p1cnt = p1->vertexCount;
	int p2cnt = p2->vertexCount;
	Vector2<NumType> ipoint;
	for (int i = p1cnt - 1, ii = 0; ii < p1cnt; i = ii++)
	{
		const Vector2<NumType>& p1b = v1[i];
		const Vector2<NumType>& p1e = v1[ii];
		for (int j = p2cnt - 1, jj = 0; jj < p2cnt; j = jj++)
		{
			const Vector2<NumType>& p2b = v2[j];
			const Vector2<NumType>& p2e = v2[jj];
			if (LinesIntersect<NumType>(p1b, p1e, p2b, p2e, ipoint) &&
				LineContains<NumType>(p1b, p1e, ipoint) &&
				LineContains<NumType>(p2b, p2e, ipoint))
				return true;
		}
	}

	return false;
}


// Определить прямоугольные границы многоугольника
template <typename NumType>
Rect<NumType> GetPolyBounds(const Poly<Vector2<NumType> >* poly)
{
	NumType minX = INFINITY;
	NumType minY = INFINITY;
	NumType maxX = -INFINITY;
	NumType maxY = -INFINITY;

	const Vector2<NumType>* ptr = poly->vertices;
	const Vector2<NumType>* end = &ptr[poly->vertexCount];
	while (ptr < end)
	{
		NumType x = ptr->x;

		if (x < minX)
			minX = x;

		if (x > maxX)
			maxX = x;

		NumType y = ptr->y;

		if (y < minY)
			minY = y;

		if (y > maxY)
			maxY = y;

		ptr++;
	}

	return Rect<NumType>(minX, minY, maxX, maxY);
}

#endif /* GEO_H_ */
