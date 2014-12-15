/*
 * GLActivity.cpp
 *
 *  С++ версия класса Activity.
 *
 *  Дата создания: 02.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#include "GLActivity.h"

GLActivity::~GLActivity()
{
	delete bitmapFactory;
	delete assets;
}

GLActivity::GLActivity(JNIEnv* env, jobject jActivity) : GLScreen(NULL)
{
	terminating = false;
	bitmapFactory = new BitmapFactory(env);
	assets = new AssetManager(env, jActivity);
	javaEnvironment = env;
	javaActivity = jActivity;
	SetParent(this);
}

void GLActivity::Terminate()
{
	if (!terminating)
	{
		terminating = true;
		JNIEnv* env = javaEnvironment;
		jobject act = javaActivity;
		// вызов метода finish класса Activity из Java-среды
		env->CallVoidMethod(act, env->GetMethodID(env->GetObjectClass(act), "finish", "()V"));
	}
}

void GLActivity::Pause()
{
	JNIEnv* env = javaEnvironment;
	jobject act = javaActivity;
	// вызов метода onPause класса Activity из Java-среды
	env->CallVoidMethod(act, env->GetMethodID(env->GetObjectClass(act), "onPause", "()V"));
}

void GLActivity::Resume()
{
	JNIEnv* env = javaEnvironment;
	jobject act = javaActivity;
	// вызов метода onResume класса Activity из Java-среды
	env->CallVoidMethod(act, env->GetMethodID(env->GetObjectClass(act), "onResume", "()V"));
}



