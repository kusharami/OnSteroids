/*
 * GLScreen.h
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#ifndef GLSCREEN_H_
#define GLSCREEN_H_

#include <jni.h>
#include <time.h>

#include "List.h"
#include "Texture.h"
#include "BitmapTexture.h"

class GLScreen
{
	public:
		int viewWidth;
		int viewHeight;
		int width;
		int height;
		float scaleX;
		float scaleY;

		List* textures;

		BitmapTexture* AddBitmapTexture(Bitmap* bitmap, int minFilter, int magFilter, bool freeBitmap);
		Texture* AddCreatedTexture(Texture* texture);

		void PrepareTextures();
		void DisposeTextures();

		virtual void OnSurfaceCreated()	{	PrepareTextures();	};
		virtual void OnSurfaceChanged(int width, int height);
		virtual void OnDrawFrame();
		virtual void OnActivityPause()	{	DisposeTextures();	};
		virtual void OnActivityResume() {};

		void RenderPause();
		void RenderResume();

		GLScreen(void* parent);
		virtual ~GLScreen()
		{
			delete textures;
		};

	protected:
		void* parent; // GLActivity
		jobject view;
		jmethodID viewPause;
		jmethodID viewResume;
		int drawWidth;
		int drawHeight;
		struct timespec startTime;

		GLScreen() {};
		void SetParent(void* parent);
		virtual void Initialize() {};
		virtual void Update(double latency) {};
		virtual void RenderInit();
		virtual void Render() {};
		virtual void RenderDone() {};

		void setSizes(int width, int height);

		virtual int getDrawWidth()
		{
			if (drawWidth <= 0)
				return viewWidth;
			else
				return drawWidth;
		}

		virtual int getDrawHeight()
		{
			if (drawHeight <= 0)
				return viewHeight;
			else
				return drawHeight;
		}
};

#endif /* GLSCREEN_H_ */
