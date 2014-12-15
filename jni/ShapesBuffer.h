/*
 * ShapesBuffer.h
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#ifndef SHAPESBUFFER_H_
#define SHAPESBUFFER_H_

#include "RenderBuffer.h"
#include "Vector2d.h"
#include "Shape.h"

class ShapesBuffer: public RenderBuffer
{
	public:
		Texture *texture;

		ShapesBuffer() {};
		ShapesBuffer(int maxCount) { Set(NULL, maxCount); };
		ShapesBuffer(Texture *texture, int maxCount) { Set(texture, maxCount); };

		void Set(Texture *texture, int maxCount)
		{
			RenderBuffer::Set(maxCount, false, true);

			this->texture = texture;
		};

		virtual void Draw();

		GLfloat* Put(GLfloat x, GLfloat y, Shape *shape);
		GLfloat* Put(GLfloat x, GLfloat y, GLfloat scaleX, GLfloat scaleY, Shape *shape);
		GLfloat* Put(Vector2d::Rect *rect, Shape *shape);
	protected:
		virtual int VerticesMultiplier() { return 4; };

};

#endif /* SHAPESBUFFER_H_ */
