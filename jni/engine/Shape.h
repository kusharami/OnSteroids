/*
 * Shape.h
 *
 *  Шаблон класса для хранения картинки и её многоугольной формы.
 *
 *  Дата создания: 01.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "Texture.h"

// вершина с текстурными координатами
typedef struct
{
	GLfloat x, y;
	GLfloat texX, texY;
} ShapePoint;

// прямоугольник для отрисовки
typedef struct
{
	ShapePoint x1y1;
	ShapePoint x2y1;
	ShapePoint x2y2;
	ShapePoint x1y2;
} ShapeRect;

#include "Vector2.h"
#include "Poly.h"


// основан на многоугольнике
template <typename NumType>
class Shape: public Poly<Vector2<NumType> >
{
	public:
		// точка отсчёта
		Vector2<NumType> origin;

		// настоящий размер картинки
		Vector2<NumType> size;

		// размеры для вывода на экран (кратны двум)
		Vector2<NumType> GLsize;

		// левая верхняя координата в текстуре
		Vector2<NumType> lt;
		// правая нижняя координата в текстуре
		Vector2<NumType> rb;

		Shape() : Poly<Vector2<NumType> >(), origin(), size(), GLsize(), lt(), rb() {};

		// копировать форму
		virtual void Assign(const Shape* source)
		{
			origin = source->origin;
			size = source->size;
			GLsize = source->GLsize;

			lt = source->lt;
			rb = source->rb;

			Poly<Vector2<NumType> >::Set(source->vertices, source->vertexCount);
		}

		// Задать форму (упрощённые параметры)
		void Set(NumType ox, NumType oy, NumType x, NumType y, NumType w, NumType h, NumType tw, NumType th,
				const Vector2<NumType>* vertices = NULL, int vertexCount = 0)
		{
			Set(Vector2<NumType>(ox, oy),
					Vector2<NumType>(x, y),
					Vector2<NumType>(w, h),
					Vector2<NumType>(tw, th),
					vertices, vertexCount);
		}

		// Задать форму
		void Set(// точка отсчёта
					const Vector2<NumType>& origin,
				// координаты картинки в текстуре
					const Vector2<NumType>& cutOrigin,
				// размер картинки
					const Vector2<NumType>& cutSize,
				// размер текстуры
					const Vector2<NumType>& textureSize,

				// вершины многоугольника
				// если не заданы - форма прямоугольная
					const Vector2<NumType>* vertices = NULL,
					int vertexCount = 0)
		{
			// размер отрисовки кратный двум
			GLsize = Vector2<NumType>(((int) cutSize.x + 1) & ~1, ((int) cutSize.y + 1) & ~1);

			// левая верхняя координата в текстуре
			lt = cutOrigin / textureSize;
			// правая нижняя координата в текстуре
			rb = lt + (GLsize / textureSize);

			// размер картинки
			size = cutSize;

			// точка отсчёта
			this->origin = origin;

			if (vertices && vertexCount > 0)
			{
				// задаём многоугольник
				Poly<Vector2<NumType> >::Set(vertices, vertexCount);
				// перемещаем в точку отсчёта
				Poly<Vector2<NumType> >::Move(origin);
			} else
				// инициируем прямоугольну форму
				Rectangle();
		}

		// Сместить координаты вершин
		virtual void Move(const Vector2<NumType>& origin)
		{
			this->origin += origin;
			Poly<Vector2<NumType> >::Move(origin);
		}

		// Повернуть под углом
		virtual void Rotate(const Vector2<NumType>& rv)
		{
			origin.Rotate(rv);
			Poly<Vector2<NumType> >::Rotate(rv);
		}

		// Масштабировать
		virtual void Scale(const Vector2<NumType>& factor)
		{
			origin *= factor;
			size *= factor;
			GLsize *= factor;
			Poly<Vector2<NumType> >::Scale(factor);
		}

		// получить прямоугольник формы
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

		// получить прямоугольник для вывода на экран
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

		// получить масштабированный и повёрнутый прямоугольник для вывода на экран
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

		// вывести прямоугольник в буффер
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

		// Прямоугольная форма
		void Rectangle()
		{
			Poly<Vector2<NumType> >::SetVertexCount(4);
			GetShapeRect(Poly<Vector2<NumType> >::vertices);
		}

	virtual ~Shape() {};
};

typedef Shape<float> ShapeF;

#endif /* SHAPE_H_ */
