/*
 * ShapesBuffer.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#include "ShapesBuffer.h"

void ShapesBuffer::Draw()
{
	if (count > 0)
	{
		glColor4f(red, green, blue, alpha);

		glBindTexture(GL_TEXTURE_2D, texture->tex_id);

		DoDraw();

		glBindTexture(GL_TEXTURE_2D, 0);

		glColor4f(1.0, 1.0, 1.0, 1.0);
	}
}

GLfloat* ShapesBuffer::Put(GLfloat x, GLfloat y, Shape* shape)
{
	float* startPtr = this->bufferPtr;
	this->bufferPtr = shape->Put(startPtr, x, y);
	return startPtr;
}

GLfloat* ShapesBuffer::Put(GLfloat x, GLfloat y, GLfloat scaleX, GLfloat scaleY, Shape* shape)
{
	float* startPtr = this->bufferPtr;
	this->bufferPtr = shape->Put(startPtr, x, y, scaleX, scaleY);
	return startPtr;
}

GLfloat* ShapesBuffer::Put(Vector2d::Rect* rect, Shape* shape)
{
	float* startPtr = this->bufferPtr;
	this->bufferPtr = shape->Put(startPtr, rect);
	return startPtr;
}
