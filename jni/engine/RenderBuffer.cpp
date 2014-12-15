/*
 * RenderBuffer.cpp
 *
 *  Базовый буффер для отрисовки на OpenGL.
 *
 *  Дата создания: 02.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#include "RenderBuffer.h"

void RenderBuffer::Init(int maxCount, bool hasColor, bool hasTexCoords)
{
	this->maxCount = maxCount;
	// инициируем буфферы вершин, индексов, цветов и текстурных координат
	Vertices::Init(maxCount * numVertices, maxCount * numIndices, hasColor, hasTexCoords);
	// заполняем индексы
	FillIndices();
	// подготавливаем к заполнению буффера
	Prepare();
}

void RenderBuffer::SetColor(uint32_t color)
{
	SetColor(color >> 16, color >> 8, color, color >> 24);
}

void RenderBuffer::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	red = (GLfloat) r / 255.0;
	green = (GLfloat) g / 255.0;
	blue = (GLfloat) b / 255.0;
	alpha = (GLfloat) a / 255.0;
}

void RenderBuffer::SetColorF(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

void RenderBuffer::Draw()
{
	if (count > 0)
		DoDraw();
}

void RenderBuffer::FillIndices()
{
	// прямоугольник состоит из двух треугольников
	GLshort*i = this->indexBuffer;
	GLshort* end = &(i[this->maxIndices]);
	int j = 0;
	while (i < end)
	{
		*(i++) = j + 0;
		*(i++) = j + 1;
		*(i++) = j + 2;

		*(i++) = j + 2;
		*(i++) = j + 3;
		*(i++) = j + 0;
		j += 4;
	}
}

void RenderBuffer::Prepare()
{
	// указатель на начало буффера
	bufferPtr = vertexBuffer;
	count = 0;
}

void RenderBuffer::DoDraw()
{
	// начать отрисовку
	Vertices::Bind();
	// вывести на экран
	Vertices::Draw(GL_TRIANGLES, 0, count * numIndices);
	// закончить отрисовку
	Vertices::Unbind();
}

// инициализация установки цвета элемента. начало цикла
#define SETCOLOR_INIT(index) \
	if (index < 0) \
		return; \
	int cnt = numVertices; \
	GLfloat* dest = &(colorBuffer[index * cnt * 4]); \
	while (cnt-- > 0)

#define SETRED(r) *(dest++) = (GLfloat) ((uint8_t) (r)) / 255.0
#define SETGREEN(g) *(dest++) = (GLfloat) ((uint8_t) (g)) / 255.0
#define SETBLUE(b) *(dest++) = (GLfloat) ((uint8_t) (b)) / 255.0
#define SETALPHA(a) *(dest++) = (GLfloat) ((uint8_t) (a)) / 255.0

#define SETCOLOR(r, g, b, a) \
	SETRED(r); \
	SETGREEN(g); \
	SETBLUE(b); \
	SETALPHA(a);

#define SETCOLORF(r, g, b, a) \
	*(dest++) = (r); \
	*(dest++) = (g); \
	*(dest++) = (b); \
	*(dest++) = (a);

void RenderBuffer::SetShapeColor(int index, uint32_t color)
{
	SETCOLOR_INIT(index)
	{
		SETCOLOR(color >> 16, color >> 8, color, color >> 24);
	}
}

void RenderBuffer::SetShapeColor(int index, uint8_t red, uint8_t green,
		uint8_t blue, uint8_t alpha)
{
	SETCOLOR_INIT(index)
	{
		SETCOLOR(red, green, blue, alpha);
	}
}

void RenderBuffer::SetShapeRed(int index, uint8_t red)
{
	SETCOLOR_INIT(index)
	{
		SETRED(red);
		dest += 3;
	}
}

void RenderBuffer::SetShapeGreen(int index, uint8_t green)
{
	SETCOLOR_INIT(index)
	{
		dest++;
		SETGREEN(green);
		dest += 2;
	}
}

void RenderBuffer::SetShapeBlue(int index, uint8_t blue)
{
	SETCOLOR_INIT(index)
	{
		dest += 2;
		SETBLUE(blue);
		dest++;
	}
}

void RenderBuffer::SetShapeAlpha(int index, uint8_t alpha)
{
	SETCOLOR_INIT(index)
	{
		dest += 3;
		SETALPHA(alpha);
	}
}

void RenderBuffer::SetShapeColorF(int index, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	SETCOLOR_INIT(index)
	{
		SETCOLORF(red, green, blue, alpha);
	}
}

void RenderBuffer::SetShapeRedF(int index, GLfloat red)
{
	SETCOLOR_INIT(index)
	{
		*(dest++) = red;
		dest += 3;
	}
}

void RenderBuffer::SetShapeGreenF(int index, GLfloat green)
{
	SETCOLOR_INIT(index)
	{
		*++dest = green;
		dest += 3;
	}
}

void RenderBuffer::SetShapeBlueF(int index, GLfloat blue)
{
	SETCOLOR_INIT(index)
	{
		dest += 2;
		*dest++ = blue;
		dest++;
	}
}

void RenderBuffer::SetShapeAlphaF(int index, GLfloat alpha)
{
	SETCOLOR_INIT(index)
	{
		dest += 3;
		*dest++ = alpha;
	}
}

