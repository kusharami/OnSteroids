/*
 * Shape.cpp
 *
 *  Created on: 01.09.2013
 *      Author: Alicia
 */

#include "Shape.h"

Shape::Shape(int textureX, int textureY, int width, int height,
		int textureWidth, int textureHeight)
{
	Set(textureX, textureY, width, height, textureWidth, textureHeight);
}

Shape::Shape(Texture *texture, int textureX, int textureY, int width, int height)
{
	Set(textureX, textureY, width, height, texture->width, texture->height);
}

Shape::Shape(Texture *texture, int textureX, int textureY, int width, int height,
		float originX, float originY)
{
	Set(textureX, textureY, width, height, texture->width, texture->height);
	this->originX = originX;
	this->originY = originY;
}

void Shape::Set(int textureX, int textureY, int width, int height,
		int textureWidth, int textureHeight)
{
	const float fw = width;
	const float fh = height;

	this->width = fw;
	this->height = fh;

	const GLfloat tex_w = textureWidth;
	const GLfloat tex_h = textureHeight;

	const GLfloat u1 = this->u1 = ((GLfloat) textureX) / tex_w;
	const GLfloat v1 = this->v1 = ((GLfloat) textureY) / tex_h;
	this->u2 = u1 + fw / tex_w;
	this->v2 = v1 + fh / tex_h;
}

GLfloat* Shape::Put(float* vertexBuffer, GLfloat x, GLfloat y)
{
	const float x1 = x + originX;
	const float y1 = y + originY;
	const float x2 = x1 + width;
	const float y2 = y2 + height;
	*(vertexBuffer++) = x1;
	*(vertexBuffer++) = y1;
	*(vertexBuffer++) = u1;
	*(vertexBuffer++) = v1;
	*(vertexBuffer++) = x2;
	*(vertexBuffer++) = y1;
	*(vertexBuffer++) = u2;
	*(vertexBuffer++) = v1;
	*(vertexBuffer++) = x2;
	*(vertexBuffer++) = y2;
	*(vertexBuffer++) = u2;
	*(vertexBuffer++) = v2;
	*(vertexBuffer++) = x1;
	*(vertexBuffer++) = y2;
	*(vertexBuffer++) = u1;
	*(vertexBuffer++) = v2;
	return vertexBuffer;
}

GLfloat* Shape::Put(GLfloat* vertexBuffer, GLfloat x, GLfloat y, GLfloat scaleX, GLfloat scaleY)
{
	const GLfloat x1 = x + originX * scaleX;
	const GLfloat y1 = y + originY * scaleY;
	const GLfloat x2 = x1 + width * scaleX;
	const GLfloat y2 = y2 + height * scaleY;
	*(vertexBuffer++) = x1;
	*(vertexBuffer++) = y1;
	*(vertexBuffer++) = u1;
	*(vertexBuffer++) = v1;
	*(vertexBuffer++) = x2;
	*(vertexBuffer++) = y1;
	*(vertexBuffer++) = u2;
	*(vertexBuffer++) = v1;
	*(vertexBuffer++) = x2;
	*(vertexBuffer++) = y2;
	*(vertexBuffer++) = u2;
	*(vertexBuffer++) = v2;
	*(vertexBuffer++) = x1;
	*(vertexBuffer++) = y2;
	*(vertexBuffer++) = u1;
	*(vertexBuffer++) = v2;
	return vertexBuffer;
}

GLfloat* Shape::Put(GLfloat* vertexBuffer, Vector2d::Rect* rect)
{
	*(vertexBuffer++) = rect->x1y1.x;
	*(vertexBuffer++) = rect->x1y1.y;
	*(vertexBuffer++) = u1;
	*(vertexBuffer++) = v1;
	*(vertexBuffer++) = rect->x2y1.x;
	*(vertexBuffer++) = rect->x2y1.y;
	*(vertexBuffer++) = u2;
	*(vertexBuffer++) = v1;
	*(vertexBuffer++) = rect->x2y2.x;
	*(vertexBuffer++) = rect->x2y2.y;
	*(vertexBuffer++) = u2;
	*(vertexBuffer++) = v2;
	*(vertexBuffer++) = rect->x1y2.x;
	*(vertexBuffer++) = rect->x1y2.y;
	*(vertexBuffer++) = u1;
	*(vertexBuffer++) = v2;
	return vertexBuffer;
}


