/*
 * ShapesBuffer.cpp
 *
 *  ������ ��� ������ �������� ����� OpenGL.
 *
 *  ���� ��������: 02.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#include "ShapesBuffer.h"
#include <new>

//* ShapesBuffer class *//

void ShapesBuffer::Draw()
{
	if (count > 0)
	{
		// �������� ����
		glColor4f(red, green, blue, alpha);

		// �������������� ��������
		glBindTexture(GL_TEXTURE_2D, texture->tex_id);

		// ���������� ������
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// ����������
		DoDraw();

		// �������������� ��������
		glBindTexture(GL_TEXTURE_2D, 0);

		// ������������ ����
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}
}

int ShapesBuffer::Put(ShapeRect* source)
{
	if (count < maxCount)
	{
		ShapeRect* r = (ShapeRect*) bufferPtr;
		// ���������� ������ �� ���������
		memcpy(r, source, sizeof(ShapeRect));
		bufferPtr = (GLfloat*) (r + 1);
		return count++;
	}
	return -1;
}

int ShapesBuffer::Put(ShapeF* shape, float x, float y)
{
	if (count < maxCount)
	{
		// �������� ������� �������������� ��� ���������
		Vector2f rect[4];
		shape->GetDrawRect(&rect[0]);
		Vector2f xy = Vector2f(x, y);
		rect[0] += xy;
		rect[1] += xy;
		rect[2] += xy;
		rect[3] += xy;
		ShapeRect* r = (ShapeRect*) bufferPtr;
		// �������� �������� � ������
		shape->Put(r, rect);
		bufferPtr = (GLfloat*) (r + 1);
		return count++;
	}
	return -1;
}

int ShapesBuffer::Put(ShapeF* shape, const Vector2f* rect)
{
	if (count < maxCount)
	{
		ShapeRect* r = (ShapeRect*) bufferPtr;
		// �������� �������� � ������
		shape->Put(r, rect);
		bufferPtr = (GLfloat*) (r + 1);
		return count++;
	}
	return -1;
}

//* ShapesBufferEx class *//

ShapesBufferEx::~ShapesBufferEx()
{
	Clear();
}

void ShapesBufferEx::Clear()
{
	for (int i = priCount; --i >= 0;)
	  buffers[i].~ShapesBuffer();

	operator delete[] (buffers);
	buffers = NULL;
}

void ShapesBufferEx::Init(Texture* texture, int priCount, int maxCount, bool colors)
{
	this->colors = colors;
	delete[] buffers;
	this->priCount = priCount;

	// placement new ��� �������� ��������
	ShapesBuffer* buffers = static_cast<ShapesBuffer*>(operator new[] (priCount * sizeof(ShapesBuffer)));

	for (int i = 0; i < priCount; i++)
	{
		ShapesBuffer* buf = new (buffers + i) ShapesBuffer();
		buf->Init(texture, maxCount, colors);
	}

	this->buffers = buffers;
}

void ShapesBufferEx::Prepare()
{
	for (int i = this->priCount; --i >= 0; )
		buffers[i].Prepare();
}

void ShapesBufferEx::Draw()
{
	int cnt = this->priCount;
	for (int i = 0; i < cnt; i++)
		buffers[i].Draw();
}

void ShapesBufferEx::Draw(int from, int to, int inc)
{
	while (true)
	{
		buffers[from].Draw();
		if (from == to)
			return;
		from += inc;
	}
}
