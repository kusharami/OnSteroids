/*
 * RectBuffer.cpp
 *
 *  Буффер для отрисовки цветных заполненных прямоугольников на OpenGL.
 *
 *  Дата создания: 15.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#include "RectBuffer.h"

int RectBuffer::Put(float x, float y, float width, float height, int color)
{
	Rect<float> rect = Rect<float>(x, y, x + width, y + height);
	return Put(rect, color);
}

int RectBuffer::Put(const Rect<float>& rect, int color)
{
	if (count < maxCount)
	{
		*bufferPtr++ = rect.left;
		*bufferPtr++ = rect.top;
		*bufferPtr++ = rect.right;
		*bufferPtr++ = rect.top;
		*bufferPtr++ = rect.right;
		*bufferPtr++ = rect.bottom;
		*bufferPtr++ = rect.left;
		*bufferPtr++ = rect.bottom;

		if (color == 0)
			// если цвет не задан, задаём цвет по умолчанию
			SetShapeColorF(count, red, green, blue, alpha);
		else
			SetShapeColor(count, color);
		return count++;
	}
	return -1;
}
