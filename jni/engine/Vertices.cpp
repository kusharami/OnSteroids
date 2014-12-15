/*
 * Vertices.cpp
 *
 *  Класс вершин для отрисовки в OpenGL.
 *
 *  Дата создания: 01.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */


#include "Vertices.h"
#include <stdlib.h>

Vertices::~Vertices()
{
	delete[] vertexBuffer;
	delete[] indexBuffer;
}

void Vertices::Init(int maxVertices, int maxIndices, bool hasColor, bool hasTexCoords)
{
	delete[] vertexBuffer;
	delete[] indexBuffer;

	this->hasColor = hasColor;
	this->hasTexCoords = hasTexCoords;
	// размер вершины (если с текстурными координатами, то 4. без них - 2)
	int vertexSize = 2 + (hasTexCoords ? 2 : 0);

	this->vertexSize = vertexSize * sizeof(GLfloat);
	int sizeVertices = maxVertices * vertexSize;

	if (hasColor)
	{
		// определяем размер буффера, если можно раскрашивать элементы
		GLfloat* buf = this->vertexBuffer = new GLfloat[sizeVertices + maxVertices * 4];
		this->colorBuffer = &(buf[sizeVertices]);
	} else
	{
		this->vertexBuffer = new GLfloat[sizeVertices];
		this->colorBuffer = NULL;
	}
	if (maxIndices > 0)
		this->indexBuffer = new GLshort[maxIndices];
	else
		this->indexBuffer = NULL;

	this->maxIndices = maxIndices;
	this->maxVertices = maxVertices;
}

void Vertices::Bind()
{
	const GLfloat *v = this->vertexBuffer;
	const int vertexSize = this->vertexSize;

	// активизируем массив вершин
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, vertexSize, v);

	if (hasTexCoords)
	{
		// активизируем текстурные координаты
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, vertexSize, &(v[2]));
	}

	if (hasColor)
	{
		// активизируем массив цветов
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, colorBuffer);
	}
}

void Vertices::Unbind()
{
	if (hasColor)
		// деактивируем массив цветов
		glDisableClientState(GL_COLOR_ARRAY);

	if (hasTexCoords)
		// деактивируем текстурные координаты
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// деактивируем массив вершин
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Vertices::Draw(int primitiveType, int offset, int count)
{
	const GLshort *i = this->indexBuffer;
	if (i != NULL)
		// рисуем всё, если есть индексы
		glDrawElements(primitiveType, count, GL_UNSIGNED_SHORT, &(i[offset]));
	else
		glDrawArrays(primitiveType, offset, count);
}



