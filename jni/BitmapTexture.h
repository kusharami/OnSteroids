/*
 * BitmapTexture.h
 *
 *  Created on: 01.09.2013
 *      Author: Natalie
 */

#ifndef BITMAPTEXTURE_H_
#define BITMAPTEXTURE_H_

#include "Texture.h"
#include "Bitmap.h"

class BitmapTexture: public Texture
{
	public:
		Bitmap *bitmap;
		bool freeBitmapAfterUse;

		BitmapTexture(Bitmap *bitmap, int minF, int magF, bool freeBitmapAfterUse);
		virtual ~BitmapTexture();
	protected:
		virtual void PrepareTextureData();
};

#endif /* BITMAPTEXTURE_H_ */
