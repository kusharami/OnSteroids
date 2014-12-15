/*
 * Texture.cpp
 *
 *  Created on: 01.09.2013
 *      Author: Alicia
 */

#include "Texture.h"

void Texture::Prepare()
{
	if (!prepared)
	{
		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);

		PrepareTextureData();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		TextureConfig();

		glBindTexture(GL_TEXTURE_2D, 0);
		prepared = true;
	}
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, tex_id);
}

void Texture::Dispose()
{
	if (prepared)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &tex_id);
		prepared = false;
	}
}

Texture::~Texture()
{
	Dispose();
}

void TextureFree(Texture* texture)
{
	delete texture;
}

