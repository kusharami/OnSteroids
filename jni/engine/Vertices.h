/*
 * Vertices.h
 *
 *  ����� ������ ��� ��������� � OpenGL.
 *
 *  ���� ��������: 01.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */


#ifndef VERTICES_H_
#define VERTICES_H_

#include <GLES/gl.h>

class Vertices
{
	public:
		// ������ ������
		GLfloat* vertexBuffer;
		// ������ ��������
		GLshort* indexBuffer;
		// ������ ������
		GLfloat* colorBuffer;

		// ������ ����� �������
		int vertexSize;

		// ������������ ���-�� ������
		int maxVertices;

		// ������������ ���-�� ��������
		int maxIndices;

		// ���� �� ������ ������
		bool hasColor;

		// ���� �� ���������� ����������
		bool hasTexCoords;

		Vertices() : vertexBuffer(NULL), colorBuffer(NULL), indexBuffer(NULL),
			hasColor(false), hasTexCoords(false), vertexSize(0), maxVertices(0), maxIndices(0) {};

		virtual ~Vertices();

		// ����������������
		void Init(int maxVertices, int maxIndices, bool hasColor, bool hasTexCoords);


		// ���������������
		void Bind();

		// ����������
		void Draw(int primitiveType, int offset, int count);

		// ��������������
		void Unbind();
};

#endif /* VERTICES_H_ */
