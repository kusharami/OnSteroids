/*
 * ShapesBuffer.h
 *
 *  Буффер для вывода картинок через OpenGL.
 *
 *  Дата создания: 02.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef SHAPESBUFFER_H_
#define SHAPESBUFFER_H_

#include <stdlib.h>
#include "RenderBuffer.h"
#include "Shape.h"

class ShapesBuffer: public RenderBuffer
{
	public:
		// текстура картинок
		Texture *texture;

		// у прямоугольника 4 вершины
		ShapesBuffer() : RenderBuffer(4), texture(NULL) {};

		// Инициируем буффер отрисовки с определённой текстурой
		void Init(Texture *texture, int maxCount, bool colors)
		{
			RenderBuffer::Init(maxCount, colors, true);

			this->texture = texture;
		};

		// вывести на экран
		virtual void Draw();

		// добавить в буффер сохранённую картинку
		int Put(ShapeRect* source);

		// добавить в буффер картинку из shape, повёрнутую и масштабированную с помощью rect
		int Put(ShapeF* shape, const Vector2f* rect);

		// добавить в буффер картинку из shape по (x, y)
		int Put(ShapeF* shape, float x, float y);
};

// Буффер для вывода картинок с приоритетами
class ShapesBufferEx
{
	public:
		// массив буфферов для приоритетов
		ShapesBuffer* buffers;
		// количество приоритетов
		int priCount;
		// можно ли раскрашивать картинки
		bool colors;

		ShapesBufferEx() : buffers(NULL), priCount(0), colors(false) {};

		virtual ~ShapesBufferEx();

		// инициировать буфферы
		void Init(Texture *texture, int priCount, int maxCount, bool colors);

		// подготовить
		void Prepare();

		// отрисовать все
		void Draw();

		// отрисовать диапазон приоритетов в определённом порядке, заданном с помощью inc
		void Draw(int from, int to, int inc);

	private:
		// удалить все буфферы
		void Clear();
};

#endif /* SHAPESBUFFER_H_ */
