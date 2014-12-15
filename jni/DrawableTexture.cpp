/*
 * DrawableTexture.cpp
 *
 *  Created on: 01.09.2013
 *      Author: Alicia
 */

#include "DrawableTexture.h"

DrawableTexture::DrawableTexture(int w, int h, int minF, int magF): Texture(minF, magF)
{
	int i = 16;

	while (w > i && i < 2048)
		i <<= 1;
	w = i;

	i = 16;
	while (h > i && i < 2048)
		i <<= 1;

	h = i;
	width = w;
	height = h;
}

void DrawableTexture::PrepareTextureData()
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

void DrawableTexture::TextureConfig()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

