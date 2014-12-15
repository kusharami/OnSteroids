/*
 * Vertices.h
 *
 *  Created on: 01.09.2013
 *      Author: Alicia
 */

#ifndef VERTICES_H_
#define VERTICES_H_

#include <GLES/gl.h>

class Vertices
{
	public:
		Vertices()
		{
			vertexBuffer = NULL;
			indexBuffer = NULL;
		};
		Vertices(int maxVertices, int maxIndices, bool hasColor, bool hasTexCoords)
		{
			vertexBuffer = NULL;
			indexBuffer = NULL;
			Set(maxVertices, maxIndices, hasColor, hasTexCoords);
		};
		virtual ~Vertices();

		void Set(int maxVertices, int maxIndices, bool hasColor, bool hasTexCoords);

		void Bind();
		void Unbind();
		void Draw(int primitiveType, int offset, int count);

		bool hasColor;
		bool hasTexCoords;
		int vertexSize;
		GLfloat *vertexBuffer;
		short *indexBuffer;
		int maxVertices;
		int maxIndices;
};

#endif /* VERTICES_H_ */
