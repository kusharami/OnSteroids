/*
 * LeveledShapesBuffer.h
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#ifndef LEVELEDSHAPESBUFFER_H_
#define LEVELEDSHAPESBUFFER_H_

#include "ShapesBuffer.h"

class LeveledShapesBuffer
{
	public:
		ShapesBuffer *buffers;
		int levelsCount;

		LeveledShapesBuffer(Texture *texture, int levelsCount, int maxCount)
		{
			buffers = NULL;
			Set(texture, levelsCount, maxCount);
		};
		virtual ~LeveledShapesBuffer();

		void Set(Texture *texture, int levelsCount, int maxCount);

		void Prepare();
		void Draw();
		void Draw(int from, int to, int inc);
		GLfloat* Put(int priority, GLfloat x, GLfloat y, Shape* shape);
		GLfloat* Put(int priority, GLfloat x, GLfloat y, GLfloat scaleX, GLfloat scaleY, Shape* shape);
		GLfloat* Put(int priority, Vector2d::Rect *rect, Shape* shape);

		void Clear();
};

#endif /* LEVELEDSHAPESBUFFER_H_ */
