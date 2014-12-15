/*
 * Vertices.h
 *
 *  Класс вершин для отрисовки в OpenGL.
 *
 *  Дата создания: 01.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */


#ifndef VERTICES_H_
#define VERTICES_H_

#include <GLES/gl.h>

class Vertices
{
	public:
		// буффер вершин
		GLfloat* vertexBuffer;
		// буффер индексов
		GLshort* indexBuffer;
		// буффер цветов
		GLfloat* colorBuffer;

		// размер одной вершины
		int vertexSize;

		// максимальное кол-во вершин
		int maxVertices;

		// максимальное кол-во индексов
		int maxIndices;

		// есть ли буффер цветов
		bool hasColor;

		// есть ли текстурные координаты
		bool hasTexCoords;

		Vertices() : vertexBuffer(NULL), colorBuffer(NULL), indexBuffer(NULL),
			hasColor(false), hasTexCoords(false), vertexSize(0), maxVertices(0), maxIndices(0) {};

		virtual ~Vertices();

		// инициализировать
		void Init(int maxVertices, int maxIndices, bool hasColor, bool hasTexCoords);


		// акстивизировать
		void Bind();

		// отрисовать
		void Draw(int primitiveType, int offset, int count);

		// деактивировать
		void Unbind();
};

#endif /* VERTICES_H_ */
