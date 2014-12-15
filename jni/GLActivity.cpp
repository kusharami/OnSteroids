/*
 * GLActivity.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#include "GLActivity.h"

#include "types.h"

GLActivity::~GLActivity()
{
	delete bitmapFactory;
	delete assets;
}

GLActivity::GLActivity(JNIEnv* env, jobject jActivity) : GLScreen()
{
	terminating = false;
	bitmapFactory = new BitmapFactory(env);
	LOGI("GLActivity: bitmapFactory created");
	assets = new AssetManager(env, jActivity);
	LOGI("GLActivity: assets created");
	javaEnvironment = env;
	javaActivity = jActivity;
	SetParent(this);
	LOGI("GLActivity: GLScreen created");
	Initialize();
}

void GLActivity::Terminate()
{
	if (!terminating)
	{
		terminating = true;
		JNIEnv* env = javaEnvironment;
		jobject act = javaActivity;
		env->CallVoidMethod(act, env->GetMethodID(env->GetObjectClass(act), "finish", "()V"));
	}
}

void GLActivity::Pause()
{
	JNIEnv* env = javaEnvironment;
	jobject act = javaActivity;
	env->CallVoidMethod(act, env->GetMethodID(env->GetObjectClass(act), "onPause", "()V"));
}

void GLActivity::Resume()
{
	JNIEnv* env = javaEnvironment;
	jobject act = javaActivity;
	env->CallVoidMethod(act, env->GetMethodID(env->GetObjectClass(act), "onResume", "()V"));
}



