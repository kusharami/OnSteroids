/*
 * OnSteroidsActivity.h
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#ifndef ONSTEROIDSACTIVITY_H_
#define ONSTEROIDSACTIVITY_H_

//#define JMETHOD(name) Java_android_game_onsteroids_GameActivity_native##name

#include <jni.h>
#include "GLActivity.h"
#include "ShapesBuffer.h"

#define STATE_ERROR -1
#define STATE_INITIALIZE 0
#define STATE_RUN 1

class OnSteroidsActivity: public GLActivity
{
	public:
		OnSteroidsActivity(JNIEnv* env, jobject jActivity) : GLActivity(env, jActivity) {};
		virtual ~OnSteroidsActivity();

	protected:
		int state;
		Shape* pillShape;
		ShapesBuffer* shapes;

		virtual void Initialize();
		virtual void Update(double latency);
		virtual void Render() ;
};

extern "C"
{
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeInitialize(JNIEnv *, jobject);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeFinalize(JNIEnv *, jclass);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeSurfaceCreated(JNIEnv *, jclass);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeSurfaceChanged(JNIEnv *, jclass, jint, jint);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeDrawFrame(JNIEnv *, jclass);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativePause(JNIEnv *, jclass);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeResume(JNIEnv *, jclass);
	JNIEXPORT jint JNICALL Java_android_game_onsteroids_GameActivity_nativeKeyDown(JNIEnv *, jclass, jint, jbyteArray);
	JNIEXPORT jint JNICALL Java_android_game_onsteroids_GameActivity_nativeKeyUp(JNIEnv *, jclass, jint, jbyteArray);
	JNIEXPORT jint JNICALL Java_android_game_onsteroids_GameActivity_nativeTouchEvent(JNIEnv *, jclass, jbyteArray);
}

#endif /* ONSTEROIDSACTIVITY_H_ */
