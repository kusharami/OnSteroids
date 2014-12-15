/*
 * Texture.h
 *
 *  Created on: 01.09.2013
 *      Author: Alicia
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GLES/gl.h>

class Texture
{
	public:
		int width, height;
		GLuint tex_id;
		int minFilter;
		int magFilter;
		bool prepared;

		Texture(int minF, int magF) : minFilter(minF), magFilter(magF), width(0), height(0), tex_id(0), prepared(false) {};
		virtual ~Texture();

		void Prepare();
		void Bind();
		void Dispose();

	protected:
		virtual void PrepareTextureData() {};
		virtual void TextureConfig() {};
};

void TextureFree(Texture* texture);

#endif /* TEXTURE_H_ */
