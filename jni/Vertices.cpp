/*
 * Vertices.cpp
 *
 *  Created on: 01.09.2013
 *      Author: Alicia
 */

#include "Vertices.h"

Vertices::~Vertices()
{
	delete[] vertexBuffer;
	delete[] indexBuffer;
}

void Vertices::Set(int maxVertices, int maxIndices, bool hasColor, bool hasTexCoords)
{
	delete[] vertexBuffer;
	delete[] indexBuffer;
	this->hasColor = hasColor;
	this->hasTexCoords = hasTexCoords;
	int vertexSize = 2 + (hasColor ? 4 : 0) + (hasTexCoords ? 2 : 0);
	this->vertexSize = vertexSize * 4;
	this->vertexBuffer = new GLfloat[maxVertices * vertexSize]; //(maxVertices * vertexSize);
	if (maxIndices > 0)
		this->indexBuffer = new short[maxIndices];
	else
		this->indexBuffer = NULL;

	this->maxIndices = maxIndices;
	this->maxVertices = maxVertices;
}

void Vertices::Bind()
{
	const GLfloat *v = this->vertexBuffer;
	const int vertexSize = this->vertexSize;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, vertexSize, v);
	v += 2;

	if (hasColor)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, vertexSize, v);
		v += 4;
	}

	if (hasTexCoords)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, vertexSize, v);
	}
}

void Vertices::Unbind()
{
	if (hasTexCoords)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (hasColor)
		glDisableClientState(GL_COLOR_ARRAY);
}

void Vertices::Draw(int primitiveType, int offset, int count)
{
	const short *i = this->indexBuffer;
	if (i != NULL)
		glDrawElements(primitiveType, count, GL_UNSIGNED_SHORT, &(i[offset]));
	else
		glDrawArrays(primitiveType, offset, count);
}



