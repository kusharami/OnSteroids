/*
 * DrawableTexture.h
 *
 *  Created on: 01.09.2013
 *      Author: Natalie
 */

#ifndef DRAWABLETEXTURE_H_
#define DRAWABLETEXTURE_H_

#include "Texture.h"

class DrawableTexture: public Texture
{
	public:
		DrawableTexture(int w, int h, int minF, int magF);

	protected:
		virtual void PrepareTextureData();
		virtual void TextureConfig();
};

#endif /* DRAWABLETEXTURE_H_ */
