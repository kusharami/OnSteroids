/*
 * Shape.h
 *
 *  Created on: 01.09.2013
 *      Author: Alicia
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "Texture.h"
#include "Vector2d.h"

class Shape
{
	public:
		GLfloat width, height;
		GLfloat u1, v1;
		GLfloat u2, v2;

		GLfloat originX, originY;

		Shape() {};
		Shape(int textureX, int textureY, int width, int height, int textureWidth, int textureHeight);
		Shape(Texture *texture, int textureX, int textureY, int width, int height);
		Shape(Texture *texture, int textureX, int textureY, int width, int height, float originX, float originY);

		void Set(int textureX, int textureY, int width, int height, int textureWidth, int textureHeight);

		GLfloat* Put(GLfloat *vertexBuffer, GLfloat x, GLfloat y);
		GLfloat* Put(GLfloat *vertexBuffer, GLfloat x, GLfloat y, GLfloat scaleX, GLfloat scaleY);
		GLfloat* Put(GLfloat *vertexBuffer, Vector2d::Rect *rect);

};

#endif /* SHAPE_H_ */
