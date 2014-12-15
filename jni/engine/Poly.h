/*
 * Poly.h
 *
 *  ������� ������� ��� ������ � ����������������.
 *
 *  ���� ��������: 12.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef POLY_H_
#define POLY_H_

#include "Utils.h"

// ����� ��������������
template <typename Vector>
class Edge
{
	public:
		// �������
		Vector normal;
		// ��������� ������������ ������� � ������ ������
		typename Vector::ValueType d;

		Edge() : normal(), d(0) {};

		Edge(const Vector& normal, typename Vector::ValueType d) : normal(normal), d(d) {};

		Edge(const Vector& p1, const Vector& p2)
		{
			// ��������� ������� � ������� �����
			d = (normal = (p2 - p1).Perpendicular().Normalize()).Dot(p1);
		};
};

// �������������
template <typename Vector>
class Poly
{
	private:
		// ����
		Edge<Vector>* edges;
		// �������� �� ������ ����
		bool edgesFilled;
	public:
		// �������. ������ ���� ������ �� ������� �������
		Vector* vertices;
		// ���-�� ������
		int vertexCount;

		Poly() : vertices(NULL), vertexCount(0), edges(NULL), edgesFilled(false) {};

		Edge<Vector>* GetEdges()
		{
			if (!edgesFilled)
			{
				// ��������� ������ ����
				edgesFilled = true;
				if (edges == NULL)
					// ������ ������, ���� �� ������
					edges = new Edge<Vector>[vertexCount];
				int cnt = vertexCount;
				Vector* v = vertices;
				Edge<Vector>* e = edges;

				// ��������� � �����
				for (int a = 0, b = cnt - 1; b >= 0; a = b--)
					*e++ = Edge<Vector>(v[a], v[b]);
			}

			return edges;
		}

		// ���������� ���-�� ������
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

		// ����������� �������������
		virtual void Assign(const Poly<Vector>* source)
		{
			if (source != NULL)
				Set(source->vertices, source->vertexCount);
			else
				SetVertexCount(0);
		}

		// ����������� ������������� �� ������
		void Set(const Vector* source, int vertexCount)
		{
			if (source == vertices)
				return;

			SetVertexCount(vertexCount);

			memcpy(vertices, source, vertexCount * sizeof(Vector));
			edgesFilled = false;
		}

		// �������� ���������� ������
		virtual void Move(const Vector& origin)
		{
			Vector* dst = vertices;
			Vector* end = &dst[vertexCount];
			while (dst < end)
				*dst++ += origin;
			edgesFilled = false;
		}

		// ��������� ��� �����
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

		// �������������� �������������
		virtual void Scale(const Vector& factor)
		{
			Vector* dst = vertices;
			Vector* end = &dst[vertexCount];
			while (dst < end)
				*dst++ *= factor;
			edgesFilled = false;
		}

		#define POLY_MIN_CNT Min<int>(vertexCount, dest->vertexCount)

		// ����������� ����������� ��������� ���-�� ������ � ������ ������������
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

		// ����������� ����������� ��������� ���-�� ������ � ������ ������������ � �������� ���������� ������
		void MoveTo(Poly<Vector>* dest, const Vector& origin)
		{
			_POLY_MOVETO_LOOP_
				*dst++ = *src++ + origin;
		}

		// ����������� ����������� ��������� ���-�� ������ � ������ ������������ � ��������� ��� �����
		void RotateTo(Poly<Vector>* dest, const Vector& angleVector)
		{
			_POLY_MOVETO_LOOP_
				*dst++ = src++->Rotate(angleVector);

		}

		// ����������� ����������� ��������� ���-�� ������ � ������ ������������ � ��������������
		void ScaleTo(Poly<Vector>* dest, const Vector& factor)
		{
			_POLY_MOVETO_LOOP_
				*dst++ = *src++ * factor;
		}

		// ��������� ����� �������������� �� ������������ �����
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

		// ����� ����� �� ������� ���� ����� ������� � ����� ��������������
		// ���������� ��������� ����� � ��������� � ���������� min
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

		// ��������� ��������� �� ����� ������ ����� ��������������
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

// ������ �������, ����������� ������������ �� ��� ��������������
template <typename Vector>
bool PolygonsIntersect(Poly<Vector>* p1, Poly<Vector>* p2, Edge<Vector>& result)
{
	typename Vector::ValueType min1;
	// ������� ����� ������� ����� �������������� 1 � �������������� ����� 2
	const Edge<Vector>* edge1 = p2->FindClosestEdge(p1->GetEdges(), p1->vertexCount, min1);
	if (edge1 == NULL)
		return false;

	typename Vector::ValueType min2;
	// ������� ����� ������� ����� �������������� 2 � �������������� ����� 1
	const Edge<Vector>* edge2 = p1->FindClosestEdge(p2->GetEdges(), p2->vertexCount, min2);
	if (edge2 == NULL)
		return false;

	// ���������� ����������� ��������� �� ���� ���������
	result = (min1 > min2)
		?	Edge<Vector>(edge1->normal, min1)
		:	Edge<Vector>(edge2->normal.Negative(), min2);
	return true;
}

#endif /* POLY_H_ */
