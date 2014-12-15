/*
 * Shape.h
 *
 *  ������ ������ ��� �������� �������� � � ������������� �����.
 *
 *  ���� ��������: 01.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "Texture.h"

// ������� � ����������� ������������
typedef struct
{
	GLfloat x, y;
	GLfloat texX, texY;
} ShapePoint;

// ������������� ��� ���������
typedef struct
{
	ShapePoint x1y1;
	ShapePoint x2y1;
	ShapePoint x2y2;
	ShapePoint x1y2;
} ShapeRect;

#include "Vector2.h"
#include "Poly.h"


// ������� �� ��������������
template <typename NumType>
class Shape: public Poly<Vector2<NumType> >
{
	public:
		// ����� �������
		Vector2<NumType> origin;

		// ��������� ������ ��������
		Vector2<NumType> size;

		// ������� ��� ������ �� ����� (������ ����)
		Vector2<NumType> GLsize;

		// ����� ������� ���������� � ��������
		Vector2<NumType> lt;
		// ������ ������ ���������� � ��������
		Vector2<NumType> rb;

		Shape() : Poly<Vector2<NumType> >(), origin(), size(), GLsize(), lt(), rb() {};

		// ���������� �����
		virtual void Assign(const Shape* source)
		{
			origin = source->origin;
			size = source->size;
			GLsize = source->GLsize;

			lt = source->lt;
			rb = source->rb;

			Poly<Vector2<NumType> >::Set(source->vertices, source->vertexCount);
		}

		// ������ ����� (���������� ���������)
		void Set(NumType ox, NumType oy, NumType x, NumType y, NumType w, NumType h, NumType tw, NumType th,
				const Vector2<NumType>* vertices = NULL, int vertexCount = 0)
		{
			Set(Vector2<NumType>(ox, oy),
					Vector2<NumType>(x, y),
					Vector2<NumType>(w, h),
					Vector2<NumType>(tw, th),
					vertices, vertexCount);
		}

		// ������ �����
		void Set(// ����� �������
					const Vector2<NumType>& origin,
				// ���������� �������� � ��������
					const Vector2<NumType>& cutOrigin,
				// ������ ��������
					const Vector2<NumType>& cutSize,
				// ������ ��������
					const Vector2<NumType>& textureSize,

				// ������� ��������������
				// ���� �� ������ - ����� �������������
					const Vector2<NumType>* vertices = NULL,
					int vertexCount = 0)
		{
			// ������ ��������� ������� ����
			GLsize = Vector2<NumType>(((int) cutSize.x + 1) & ~1, ((int) cutSize.y + 1) & ~1);

			// ����� ������� ���������� � ��������
			lt = cutOrigin / textureSize;
			// ������ ������ ���������� � ��������
			rb = lt + (GLsize / textureSize);

			// ������ ��������
			size = cutSize;

			// ����� �������
			this->origin = origin;

			if (vertices && vertexCount > 0)
			{
				// ����� �������������
				Poly<Vector2<NumType> >::Set(vertices, vertexCount);
				// ���������� � ����� �������
				Poly<Vector2<NumType> >::Move(origin);
			} else
				// ���������� ������������ �����
				Rectangle();
		}

		// �������� ���������� ������
		virtual void Move(const Vector2<NumType>& origin)
		{
			this->origin += origin;
			Poly<Vector2<NumType> >::Move(origin);
		}

		// ��������� ��� �����
		virtual void Rotate(const Vector2<NumType>& rv)
		{
			origin.Rotate(rv);
			Poly<Vector2<NumType> >::Rotate(rv);
		}

		// ��������������
		virtual void Scale(const Vector2<NumType>& factor)
		{
			origin *= factor;
			size *= factor;
			GLsize *= factor;
			Poly<Vector2<NumType> >::Scale(factor);
		}

		// �������� ������������� �����
		void GetShapeRect(Vector2<NumType>* dest)
		{
			NumType x1 = origin.x;
			NumType y1 = origin.y;
			NumType x2 = x1 + size.x;
			NumType y2 = y1 + size.y;
			*dest++ = Vector2<NumType>(x1, y1);
			*dest++ = Vector2<NumType>(x2, y1);
			*dest++ = Vector2<NumType>(x2, y2);
			*dest = Vector2<NumType>(x1, y2);
		}

		// �������� ������������� ��� ������ �� �����
		void GetDrawRect(Vector2<NumType>* dest)
		{
			NumType x1 = origin.x;
			NumType y1 = origin.y;
			NumType x2 = x1 + GLsize.x;
			NumType y2 = y1 + GLsize.y;
			*dest++ = Vector2<NumType>(x1, y1);
			*dest++ = Vector2<NumType>(x2, y1);
			*dest++ = Vector2<NumType>(x2, y2);
			*dest = Vector2<NumType>(x1, y2);
		}

		// �������� ���������������� � ��������� ������������� ��� ������ �� �����
		void GetDrawRect(Vector2<NumType>* dest, const Vector2<NumType>& dir, const Vector2<NumType>& scale)
		{
			NumType x1 = origin.x;
			NumType y1 = origin.y;
			NumType x2 = x1 + GLsize.x;
			NumType y2 = y1 + GLsize.y;
			*dest++ = Vector2<NumType>(x1, y1).Rotate(dir) * scale;
			*dest++ = Vector2<NumType>(x2, y1).Rotate(dir) * scale;
			*dest++ = Vector2<NumType>(x2, y2).Rotate(dir) * scale;
			*dest = Vector2<NumType>(x1, y2).Rotate(dir) * scale;
		}

		// ������� ������������� � ������
		void Put(ShapeRect* dest, const Vector2<NumType>* rect) const
		{
			#define __PUT_SETPOINT(p, u, v) \
				p.x = rect->x; \
				p.y = rect++->y; \
				p.texX = u; \
				p.texY = v;

			__PUT_SETPOINT(dest->x1y1, lt.x, lt.y);
			__PUT_SETPOINT(dest->x2y1, rb.x, lt.y);
			__PUT_SETPOINT(dest->x2y2, rb.x, rb.y);
			__PUT_SETPOINT(dest->x1y2, lt.x, rb.y);

			#undef __PUT_SETPOINT
		}

		// ������������� �����
		void Rectangle()
		{
			Poly<Vector2<NumType> >::SetVertexCount(4);
			GetShapeRect(Poly<Vector2<NumType> >::vertices);
		}

	virtual ~Shape() {};
};

typedef Shape<float> ShapeF;

#endif /* SHAPE_H_ */
