/*
 * ShapesBuffer.h
 *
 *  ������ ��� ������ �������� ����� OpenGL.
 *
 *  ���� ��������: 02.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef SHAPESBUFFER_H_
#define SHAPESBUFFER_H_

#include <stdlib.h>
#include "RenderBuffer.h"
#include "Shape.h"

class ShapesBuffer: public RenderBuffer
{
	public:
		// �������� ��������
		Texture *texture;

		// � �������������� 4 �������
		ShapesBuffer() : RenderBuffer(4), texture(NULL) {};

		// ���������� ������ ��������� � ����������� ���������
		void Init(Texture *texture, int maxCount, bool colors)
		{
			RenderBuffer::Init(maxCount, colors, true);

			this->texture = texture;
		};

		// ������� �� �����
		virtual void Draw();

		// �������� � ������ ���������� ��������
		int Put(ShapeRect* source);

		// �������� � ������ �������� �� shape, ��������� � ���������������� � ������� rect
		int Put(ShapeF* shape, const Vector2f* rect);

		// �������� � ������ �������� �� shape �� (x, y)
		int Put(ShapeF* shape, float x, float y);
};

// ������ ��� ������ �������� � ������������
class ShapesBufferEx
{
	public:
		// ������ �������� ��� �����������
		ShapesBuffer* buffers;
		// ���������� �����������
		int priCount;
		// ����� �� ������������ ��������
		bool colors;

		ShapesBufferEx() : buffers(NULL), priCount(0), colors(false) {};

		virtual ~ShapesBufferEx();

		// ������������ �������
		void Init(Texture *texture, int priCount, int maxCount, bool colors);

		// �����������
		void Prepare();

		// ���������� ���
		void Draw();

		// ���������� �������� ����������� � ����������� �������, �������� � ������� inc
		void Draw(int from, int to, int inc);

	private:
		// ������� ��� �������
		void Clear();
};

#endif /* SHAPESBUFFER_H_ */
