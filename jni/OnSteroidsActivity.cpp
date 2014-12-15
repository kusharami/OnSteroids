/*
 * OnSteroidsActivity.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#include "OnSteroidsActivity.h"
#include "AssetManager.h"

OnSteroidsActivity::~OnSteroidsActivity()
{
	delete shapes;
	delete pillShape;
}

void OnSteroidsActivity::Initialize()
{
	AssetFile* file = assets->Open("gfx/shapes.png");
	try
	{
		int size = file->Size();
		void* buf = malloc(size);
		try
		{
			file->Read(buf, size);

			Texture* texture = this->AddBitmapTexture(bitmapFactory->Decode(buf, size), GL_NEAREST, GL_NEAREST, true);

			this->pillShape = new Shape(texture, 0, 0, 48, 96, 46.0 / 2.0, 95.0 / 2.0);

			this->shapes = new ShapesBuffer(texture, 1);

			this->state = STATE_INITIALIZE;
		} catch (void* e)
		{
			state = STATE_ERROR;
		}
		free(buf);
	} catch (void* e)
	{
		state = STATE_ERROR;
	}
	delete file;
}

void OnSteroidsActivity::Update(double latency)
{
	switch (state)
	{
		case STATE_ERROR:
			Terminate();
			break;

		case STATE_INITIALIZE:
			shapes->Put(width / 2, height / 2, pillShape);
			state = STATE_RUN;

		case STATE_RUN:
			break;
	}
}

void OnSteroidsActivity::Render()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	shapes->Draw();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}


OnSteroidsActivity* activity;
GLScreen* screen;
InputListener* input;

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeInitialize(JNIEnv *env, jobject jActivity)
{
	activity = new OnSteroidsActivity(env, jActivity);
//	screen = activity;
//	input = activity;
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeFinalize(JNIEnv *env, jclass)
{
//	delete activity;
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeSurfaceCreated(JNIEnv *env, jclass)
{
//	screen->OnSurfaceCreated();
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeSurfaceChanged(JNIEnv *env, jclass, jint width, jint height)
{
//	screen->OnSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeDrawFrame(JNIEnv *env, jclass)
{
//	screen->OnDrawFrame();
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativePause(JNIEnv *env, jclass)
{
//	screen->OnActivityPause();
}

JNIEXPORT void JNICALL Java_android_game_onsteroids_GameActivity_nativeResume(JNIEnv *env, jclass)
{
//	screen->OnActivityResume();
}

JNIEXPORT jint JNICALL Java_android_game_onsteroids_GameActivity_nativeKeyDown(JNIEnv *env, jclass, jint keyCode, jbyteArray data)
{
//	return input->KeyDown(keyCode, (KeyEvent*) env->GetByteArrayElements(data, 0));
}

JNIEXPORT jint JNICALL Java_android_game_onsteroids_GameActivity_nativeKeyUp(JNIEnv *env, jclass, jint keyCode, jbyteArray data)
{
//	return input->KeyUp(keyCode, (KeyEvent*) env->GetByteArrayElements(data, 0));
}

JNIEXPORT jint JNICALL Java_android_game_onsteroids_GameActivity_nativeTouchEvent(JNIEnv *env, jclass, jbyteArray data)
{
//	return input->TouchEvent((MotionEvent*) env->GetByteArrayElements(data, 0));
}

