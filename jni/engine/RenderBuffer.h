/*
 * RenderBuffer.h
 *
 *  ������� ������ ��� ��������� �� OpenGL.
 *
 *  ���� ��������: 02.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef RENDERBUFFER_H_
#define RENDERBUFFER_H_

#include <GLES/gl.h>
#include "Vertices.h"

typedef struct
{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;
} GLcolor;

static inline GLfloat ColorToFloat(uint8_t color)
{
	return (GLfloat) color / 255.0;
}

class RenderBuffer: public Vertices
{
	public:
		// ���-�� ���������
		int count;

		// ������������ ���-�� ���������
		int maxCount;

		// ���-�� ������ ������ ��������
		int numVertices;

		// ���-�� �������� ������ ��������
		int numIndices;

		// ������������� ���� �� ��� ��������
		GLfloat red;
		GLfloat green;
		GLfloat blue;
		GLfloat alpha;

		RenderBuffer(int numVertices) : Vertices(), numVertices(numVertices), numIndices(6), maxCount(0),
				red(1.0), green(1.0), blue(1.0), alpha(1.0), count(0), bufferPtr(NULL) {};

		// ���������������� ������ ������������ �������
		// � ������������ ����������� ����� � hasColor,
		// � ����������� ������������ � hasTexCoords.
		void Init(int maxCount, bool hasColor, bool hasTexCoords);

		// ���������� ������������� ����
		void SetColor(uint32_t color);
		void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetColorF(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

		// ������� �� �����
		virtual void Draw();

		// �������� ������
		virtual void Prepare();

		// ���������� ���� ��������, ���� hasColor == true
		void SetShapeColor(int index, uint32_t color);

		void SetShapeColor(int index, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
		void SetShapeRed(int index, uint8_t red);
		void SetShapeGreen(int index, uint8_t green);
		void SetShapeBlue(int index, uint8_t blue);
		void SetShapeAlpha(int index, uint8_t alpha);

		void SetShapeColorF(int index, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
		void SetShapeRedF(int index, GLfloat red);
		void SetShapeGreenF(int index, GLfloat green);
		void SetShapeBlueF(int index, GLfloat blue);
		void SetShapeAlphaF(int index, GLfloat alpha);

	protected:
		// ��������� �� ��������� ������� � �������
		GLfloat* bufferPtr;

		// ��������� �������
		virtual void FillIndices();

		// ���������
		virtual void DoDraw();
};

#endif /* RENDERBUFFER_H_ */
