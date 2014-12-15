/*
 * AndroidMain.h
 *
 *  Функции для вызова из Java-среды.
 *
 *  Дата создания: 01.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef ANDROIDMAIN_H_
#define ANDROIDMAIN_H_

#include <jni.h>
#include <stdlib.h>

extern "C"
{
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeInitialize(JNIEnv *, jobject);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeFinalize(JNIEnv *, jclass);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeSurfaceCreated(JNIEnv *, jclass);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeSurfaceChanged(JNIEnv *, jclass, jint, jint);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeDrawFrame(JNIEnv *, jclass);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativePause(JNIEnv *, jclass);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeResume(JNIEnv *, jclass);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeKeyEvent(JNIEnv *, jclass, jbyteArray);
	JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeTouchEvent(JNIEnv *, jclass, jbyteArray);
}

#endif /* ANDROIDMAIN_H_ */
