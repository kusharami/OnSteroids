/*
 * LeveledShapesBuffer.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */
//OnSteroidsActivity.cpp \
//										GLActivity.cpp \
//										GLScreen.cpp \
//										AssetManager.cpp \
//										Bitmap.cpp \
//										BitmapTexture.cpp \
//										DrawableTexture.cpp \
//										LeveledShapesBuffer.cpp \
//										List.cpp \
//										RenderBuffer.cpp \
//										Shape.cpp \
//										ShapesBuffer.cpp \
//										Texture.cpp \
//										Vertices.cpp
#include "LeveledShapesBuffer.h"
#include <new>

LeveledShapesBuffer::~LeveledShapesBuffer()
{
	Clear();
}

void LeveledShapesBuffer::Clear()
{
	for (int i = levelsCount; --i >= 0;)
	  buffers[i].~ShapesBuffer();

	operator delete[] (buffers);
	buffers = NULL;
}

void LeveledShapesBuffer::Set(Texture* texture, int levelsCount, int maxCount)
{
	delete[] buffers;
	this->levelsCount = levelsCount;

	ShapesBuffer* buffers = static_cast<ShapesBuffer*>(operator new[] (levelsCount * sizeof(ShapesBuffer)));
	//while (--levelsCount >= 0)
	for (int i = 0; i < levelsCount; i++)
		new (buffers + i) ShapesBuffer(texture, maxCount);

	this->buffers = buffers;
}

void LeveledShapesBuffer::Prepare()
{
	for (int i = this->levelsCount; --i >= 0; )
		buffers[i].Prepare();
}

void LeveledShapesBuffer::Draw()
{
	for (int i = this->levelsCount; --i >= 0; )
		buffers[i].Draw();
}

void LeveledShapesBuffer::Draw(int from, int to, int inc)
{
	while (true)
	{
		buffers[from].Draw();
		if (from == to)
			return;
		from += inc;
	}
}

GLfloat* LeveledShapesBuffer::Put(int priority, GLfloat x, GLfloat y, Shape* shape)
{
	return buffers[priority].Put(x, y, shape);
}

GLfloat* LeveledShapesBuffer::Put(int priority, GLfloat x, GLfloat y,
		GLfloat scaleX, GLfloat scaleY, Shape* shape)
{
	return buffers[priority].Put(x, y, scaleX, scaleY, shape);
}

GLfloat* LeveledShapesBuffer::Put(int priority, Vector2d::Rect* rect, Shape* shape)
{
	return buffers[priority].Put(rect, shape);
}


