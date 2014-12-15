/*
 * GLActivity.h
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#ifndef GLACTIVITY_H_
#define GLACTIVITY_H_

#include <jni.h>
#include <stdlib.h>

#include "Bitmap.h"
#include "AssetManager.h"
#include "GLScreen.h"
#include "InputListener.h"

//#ifndef JMETHODPREFIX_H
//#define JMETHOD(name) Java_com_example_Demo_MainActivity_native##name
//#endif
//
//#ifndef GL_ACTIVITY
//#define GL_ACTIVITY GLActivity
//#endif
//
//#ifdef GL_ACTIVITY_H
//#include GL_ACTIVITY_H
//#endif

class GLActivity: public GLScreen, public InputListener
{
	public:
		JNIEnv* javaEnvironment;
		jobject javaActivity;
		BitmapFactory* bitmapFactory;
		AssetManager* assets;

		virtual ~GLActivity();
		GLActivity(JNIEnv* env, jobject jActivity);

		void Terminate();
		void Pause();
		void Resume();

	protected:
		bool terminating;
};
//extern "C"
//{
//	JNIEXPORT void JNICALL JMETHOD(Initialize)(JNIEnv *, jobject);
//	JNIEXPORT void JNICALL JMETHOD(Finalize)(JNIEnv *, jclass);
//	JNIEXPORT void JNICALL JMETHOD(SurfaceCreated)(JNIEnv *, jclass);
//	JNIEXPORT void JNICALL JMETHOD(SurfaceChanged)(JNIEnv *, jclass, jint, jint);
//	JNIEXPORT void JNICALL JMETHOD(DrawFrame)(JNIEnv *, jclass);
//	JNIEXPORT void JNICALL JMETHOD(Pause)(JNIEnv *, jclass);
//	JNIEXPORT void JNICALL JMETHOD(Resume)(JNIEnv *, jclass);
//	JNIEXPORT jint JNICALL JMETHOD(KeyDown)(JNIEnv *, jclass, jint, jbyteArray);
//	JNIEXPORT jint JNICALL JMETHOD(KeyUp)(JNIEnv *, jclass, jint, jbyteArray);
//	JNIEXPORT jint JNICALL JMETHOD(TouchEvent)(JNIEnv *, jclass, jbyteArray);
//}

#endif /* GLACTIVITY_H_ */
