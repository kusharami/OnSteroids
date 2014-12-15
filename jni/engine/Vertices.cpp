/*
 * Vertices.cpp
 *
 *  ����� ������ ��� ��������� � OpenGL.
 *
 *  ���� ��������: 01.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
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
	// ������ ������� (���� � ����������� ������������, �� 4. ��� ��� - 2)
	int vertexSize = 2 + (hasTexCoords ? 2 : 0);

	this->vertexSize = vertexSize * sizeof(GLfloat);
	int sizeVertices = maxVertices * vertexSize;

	if (hasColor)
	{
		// ���������� ������ �������, ���� ����� ������������ ��������
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

	// ������������ ������ ������
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, vertexSize, v);

	if (hasTexCoords)
	{
		// ������������ ���������� ����������
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, vertexSize, &(v[2]));
	}

	if (hasColor)
	{
		// ������������ ������ ������
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, colorBuffer);
	}
}

void Vertices::Unbind()
{
	if (hasColor)
		// ������������ ������ ������
		glDisableClientState(GL_COLOR_ARRAY);

	if (hasTexCoords)
		// ������������ ���������� ����������
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// ������������ ������ ������
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Vertices::Draw(int primitiveType, int offset, int count)
{
	const GLshort *i = this->indexBuffer;
	if (i != NULL)
		// ������ ��, ���� ���� �������
		glDrawElements(primitiveType, count, GL_UNSIGNED_SHORT, &(i[offset]));
	else
		glDrawArrays(primitiveType, offset, count);
}



