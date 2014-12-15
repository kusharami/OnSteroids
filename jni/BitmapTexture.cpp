/*
 * BitmapTexture.cpp
 *
 *  Created on: 01.09.2013
 *      Author: Alicia
 */

#include "BitmapTexture.h"

BitmapTexture::BitmapTexture(Bitmap *bitmap, int minF, int magF, bool freeBitmapAfterUse) : Texture(minF, magF)
{
	this->width = bitmap->info.width;
	this->height = bitmap->info.height;
	this->bitmap = bitmap;
}

BitmapTexture::~BitmapTexture()
{
	if (freeBitmapAfterUse)
		delete bitmap;
}

void BitmapTexture::PrepareTextureData()
{
	Bitmap *bitmap = this->bitmap;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->info.width, bitmap->info.height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, bitmap->pixels);
}


