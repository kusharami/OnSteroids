/*
 * RectBuffer.h
 *
 *  Буффер для отрисовки цветных заполненных прямоугольников на OpenGL.
 *
 *  Дата создания: 15.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef RECTBUFFER_H_
#define RECTBUFFER_H_

#include "RenderBuffer.h"
#include "Rect.h"

class RectBuffer: public RenderBuffer
{
	public:
		// У прямоугольника 4 вершины
		RectBuffer() : RenderBuffer(4) {};

		void Init(int maxCount)
		{
			// Создаём буффер отрисовки с цветами, но без текстурных координат
			RenderBuffer::Init(maxCount, true, false);
		};

		// добавить прямоугольник с координатами (x, y) определённого цвета и размера
		int Put(float x, float y, float width, float height, int color);

		// добавить прямоугольник, описанный объектом rect, определённого цвета
		int Put(const Rect<float>& rect, int color);
};

#endif /* RECTBUFFER_H_ */
