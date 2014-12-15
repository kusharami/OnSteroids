/*
 * RenderBuffer.h
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#ifndef RENDERBUFFER_H_
#define RENDERBUFFER_H_

#include <GLES/gl.h>
#include "Vertices.h"

class RenderBuffer: public Vertices
{
	public:
		int count;

		GLfloat red;
		GLfloat green;
		GLfloat blue;
		GLfloat alpha;

		RenderBuffer()
		{
			red = 1.0;
			green = 1.0;
			blue = 1.0;
			alpha = 1.0;
		};
		RenderBuffer(int maxCount, bool hasColor, bool hasTexCoords)
		{
			RenderBuffer();
			Set(maxCount, hasColor, hasTexCoords);
		};

		void Set(int maxCount, bool hasColor, bool hasTexCoords);

		void SetColor(uint32_t color);
		void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetColorF(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

		virtual void Draw();
		virtual void Prepare();

	protected:
		GLfloat *bufferPtr;
		int triCount;

		virtual int VerticesMultiplier() { return 1; };
		virtual int IndicesMultiplier() { return 6; };
		virtual void FillIndices();
		virtual void DoDraw();
};

#endif /* RENDERBUFFER_H_ */
