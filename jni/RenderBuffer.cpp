/*
 * RenderBuffer.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#include "RenderBuffer.h"

void RenderBuffer::Set(int maxCount, bool hasColor, bool hasTexCoords)
{
	Vertices::Set(maxCount * VerticesMultiplier(), maxCount * IndicesMultiplier(), hasColor, hasTexCoords);
	FillIndices();
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
	short *i = this->indexBuffer;
	int count = this->maxIndices;
	int j = 0;
	while (--count >= 0)
	{
		*(i++) = (short) (j + 0);
		*(i++) = (short) (j + 1);
		*(i++) = (short) (j + 2);

		*(i++) = (short) (j + 2);
		*(i++) = (short) (j + 3);
		*(i++) = (short) (j + 0);
		j += 4;
	}
}

void RenderBuffer::Prepare()
{
	bufferPtr = vertexBuffer;
	count = 0;
	triCount = 0;
}

void RenderBuffer::DoDraw()
{
	Vertices::Bind();
	Vertices::Draw(GL_TRIANGLES, 0, triCount);
	Vertices::Unbind();
}


