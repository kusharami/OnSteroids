/*
 * AndroidMain.cpp
 *
 *  ������� ��� ������ �� Java-�����.
 *
 *  ���� ��������: 01.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#include "AndroidMain.h"

#include "OnSteroidsActivity.h"

// ������� �����
OnSteroidsActivity* activity;

// �������� �����
GLScreen* screen;

// ����
InputListener* input;

// ���������������� �� ���������
bool initialized = false;

// �� ����� ��
bool paused = false;

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeInitialize(JNIEnv *env, jobject jActivity)
{
	if (!initialized)
	{
		initialized = true;
		activity = new OnSteroidsActivity(env, jActivity);
		screen = activity;
		input = activity;
	}
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeFinalize(JNIEnv *env, jclass)
{
	if (initialized)
		delete activity;
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeSurfaceCreated(JNIEnv *env, jclass)
{
	screen->OnSurfaceCreated();
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeSurfaceChanged(JNIEnv *env, jclass, jint width, jint height)
{
	screen->OnSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeDrawFrame(JNIEnv *env, jclass)
{
	screen->OnDrawFrame();
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativePause(JNIEnv *env, jclass)
{
	if (initialized & !paused)
	{
		paused = true;
		screen->OnActivityPause();
	}
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeResume(JNIEnv *env, jclass)
{
	if (paused)
	{
		paused = false;
		screen->OnActivityResume();
	}
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeKeyEvent(JNIEnv *env, jclass, jbyteArray data)
{
	KeyEventRec event;
	env->GetByteArrayRegion(data, 0, env->GetArrayLength(data), (jbyte*) &event);
	input->KeyEvent(&event);
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeTouchEvent(JNIEnv *env, jclass, jbyteArray data)
{
	TouchEventRec event;
	env->GetByteArrayRegion(data, 0, env->GetArrayLength(data), (jbyte*) &event);
	input->TouchEvent(&event);
}


