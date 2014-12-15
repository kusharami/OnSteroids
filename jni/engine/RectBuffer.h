/*
 * RectBuffer.h
 *
 *  ������ ��� ��������� ������� ����������� ��������������� �� OpenGL.
 *
 *  ���� ��������: 15.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef RECTBUFFER_H_
#define RECTBUFFER_H_

#include "RenderBuffer.h"
#include "Rect.h"

class RectBuffer: public RenderBuffer
{
	public:
		// � �������������� 4 �������
		RectBuffer() : RenderBuffer(4) {};

		void Init(int maxCount)
		{
			// ������ ������ ��������� � �������, �� ��� ���������� ���������
			RenderBuffer::Init(maxCount, true, false);
		};

		// �������� ������������� � ������������ (x, y) ������������ ����� � �������
		int Put(float x, float y, float width, float height, int color);

		// �������� �������������, ��������� �������� rect, ������������ �����
		int Put(const Rect<float>& rect, int color);
};

#endif /* RECTBUFFER_H_ */
