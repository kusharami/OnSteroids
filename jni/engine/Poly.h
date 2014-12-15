/*
 * Poly.h
 *
 *  Шаблоны классов для работы с многоугольниками.
 *
 *  Дата создания: 12.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef POLY_H_
#define POLY_H_

#include "Utils.h"

// Ребро многоугольника
template <typename Vector>
class Edge
{
	public:
		// нормаль
		Vector normal;
		// скалярное произведение нормали с первой точкой
		typename Vector::ValueType d;

		Edge() : normal(), d(0) {};

		Edge(const Vector& normal, typename Vector::ValueType d) : normal(normal), d(d) {};

		Edge(const Vector& p1, const Vector& p2)
		{
			// получение нормали и скаляра ребра
			d = (normal = (p2 - p1).Perpendicular().Normalize()).Dot(p1);
		};
};

// многоугольник
template <typename Vector>
class Poly
{
	private:
		// рёбра
		Edge<Vector>* edges;
		// заполнен ли массив рёбер
		bool edgesFilled;
	public:
		// вершины. должны быть заданы по часовой стрелке
		Vector* vertices;
		// кол-во вершин
		int vertexCount;

		Poly() : vertices(NULL), vertexCount(0), edges(NULL), edgesFilled(false) {};

		Edge<Vector>* GetEdges()
		{
			if (!edgesFilled)
			{
				// заполняем массив рёбер
				edgesFilled = true;
				if (edges == NULL)
					// создаём массив, если не создан
					edges = new Edge<Vector>[vertexCount];
				int cnt = vertexCount;
				Vector* v = vertices;
				Edge<Vector>* e = edges;

				// заполняем с конца
				for (int a = 0, b = cnt - 1; b >= 0; a = b--)
					*e++ = Edge<Vector>(v[a], v[b]);
			}

			return edges;
		}

		// Уствновить кол-во вершин
		void SetVertexCount(int cnt)
		{
			if (cnt != vertexCount)
			{
				delete[] vertices;
				delete[] edges;
				if (cnt > 0)
				{
					vertices = new Vector[cnt];
					vertexCount = cnt;
				} else
				{
					vertices = NULL;
					vertexCount = 0;
				}
				edges = NULL;
				edgesFilled = false;
			}
		}

		// Скопировать многоугольник
		virtual void Assign(const Poly<Vector>* source)
		{
			if (source != NULL)
				Set(source->vertices, source->vertexCount);
			else
				SetVertexCount(0);
		}

		// Скопировать многоугольник из памяти
		void Set(const Vector* source, int vertexCount)
		{
			if (source == vertices)
				return;

			SetVertexCount(vertexCount);

			memcpy(vertices, source, vertexCount * sizeof(Vector));
			edgesFilled = false;
		}

		// Сместить координаты вершин
		virtual void Move(const Vector& origin)
		{
			Vector* dst = vertices;
			Vector* end = &dst[vertexCount];
			while (dst < end)
				*dst++ += origin;
			edgesFilled = false;
		}

		// Повернуть под углом
		virtual void Rotate(const Vector& angleVector)
		{
			Vector* dst = vertices;
			Vector* end = &dst[vertexCount];
			while (dst < end)
			{
				*dst = dst->Rotate(angleVector);
				dst++;
			}
			edgesFilled = false;
		}

		// Масштабировать многоугольник
		virtual void Scale(const Vector& factor)
		{
			Vector* dst = vertices;
			Vector* end = &dst[vertexCount];
			while (dst < end)
				*dst++ *= factor;
			edgesFilled = false;
		}

		#define POLY_MIN_CNT Min<int>(vertexCount, dest->vertexCount)

		// Скопировать максимально возможное кол-во вершин в другой многоугольни
		void CopyTo(Poly<Vector>* dest)
		{
			memcpy(dest->vertices, vertices, sizeof(Vector) * POLY_MIN_CNT);
			dest->edgesFilled = false;
		}

		#define _POLY_MOVETO_LOOP_ \
			dest->edgesFilled = false; \
			Vector* dst = dest->vertices; \
			Vector* src = vertices; \
			Vector* end = &src[POLY_MIN_CNT]; \
			while (src < end)

		// Скопировать максимально возможное кол-во вершин в другой многоугольни и сместить координаты вершин
		void MoveTo(Poly<Vector>* dest, const Vector& origin)
		{
			_POLY_MOVETO_LOOP_
				*dst++ = *src++ + origin;
		}

		// Скопировать максимально возможное кол-во вершин в другой многоугольни и повернуть под углом
		void RotateTo(Poly<Vector>* dest, const Vector& angleVector)
		{
			_POLY_MOVETO_LOOP_
				*dst++ = src++->Rotate(angleVector);

		}

		// Скопировать максимально возможное кол-во вершин в другой многоугольни и масштабировать
		void ScaleTo(Poly<Vector>* dest, const Vector& factor)
		{
			_POLY_MOVETO_LOOP_
				*dst++ = *src++ * factor;
		}

		// Дистанция этого многоугольника до определённого ребра
		typename Vector::ValueType EdgeDistance(const Edge<Vector>& edge) const
		{
			Vector* v = vertices;
			Vector* end = &v[vertexCount];
			const Vector& normal = edge.normal;
			typename Vector::ValueType min = INFINITY;
			while (v < end)
				min = Min<typename Vector::ValueType>(min, normal.Dot(*v++));

			return min - edge.d;
		}

		// Найти ребро из массива рёбер самое близкое к этому многоугольнику
		// возвращает найденное ребро и дистанцию в переменную min
		const Edge<Vector>* FindClosestEdge(const Edge<Vector>* edges, int edgeCount,
				typename Vector::ValueType& min) const
		{
			int idx = -1;
			typename Vector::ValueType m = -INFINITY;
			for (int i = 0; i < edgeCount; i++)
			{
				typename Vector::ValueType dist = EdgeDistance(edges[i]);
				if (dist > 0)
					return NULL;
				else
				if (dist > m)
				{
					m = dist;
					idx = i;
				}
			}
			if (idx < 0)
				return NULL;
			min = m;
			return &edges[idx];
		}

		// Проверить находится ли точка внутри этого многоугольника
		bool Contains(const Vector& point)
		{
			Edge<Vector>* e = GetEdges();
			Edge<Vector>* end = &e[vertexCount];
			while (e < end)
			{
				if (e->normal.Dot(point) - e->d > 0)
					return false;
				e++;
			}
			return true;
		}

		virtual ~Poly()
		{
			delete[] vertices;
			delete[] edges;
		}
};

// Шаблон функции, проверяющей пересекаются ли два многоугольника
template <typename Vector>
bool PolygonsIntersect(Poly<Vector>* p1, Poly<Vector>* p2, Edge<Vector>& result)
{
	typename Vector::ValueType min1;
	// находим самое близкое ребро многоугольника 1 к многоугольнику номер 2
	const Edge<Vector>* edge1 = p2->FindClosestEdge(p1->GetEdges(), p1->vertexCount, min1);
	if (edge1 == NULL)
		return false;

	typename Vector::ValueType min2;
	// находим самое близкое ребро многоугольника 2 к многоугольнику номер 1
	const Edge<Vector>* edge2 = p1->FindClosestEdge(p2->GetEdges(), p2->vertexCount, min2);
	if (edge2 == NULL)
		return false;

	// возвращаем минимальную дистанцию из двух найденных
	result = (min1 > min2)
		?	Edge<Vector>(edge1->normal, min1)
		:	Edge<Vector>(edge2->normal.Negative(), min2);
	return true;
}

#endif /* POLY_H_ */
