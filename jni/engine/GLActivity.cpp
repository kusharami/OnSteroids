/*
 * GLActivity.cpp
 *
 *  �++ ������ ������ Activity.
 *
 *  ���� ��������: 02.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
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
		// ����� ������ finish ������ Activity �� Java-�����
		env->CallVoidMethod(act, env->GetMethodID(env->GetObjectClass(act), "finish", "()V"));
	}
}

void GLActivity::Pause()
{
	JNIEnv* env = javaEnvironment;
	jobject act = javaActivity;
	// ����� ������ onPause ������ Activity �� Java-�����
	env->CallVoidMethod(act, env->GetMethodID(env->GetObjectClass(act), "onPause", "()V"));
}

void GLActivity::Resume()
{
	JNIEnv* env = javaEnvironment;
	jobject act = javaActivity;
	// ����� ������ onResume ������ Activity �� Java-�����
	env->CallVoidMethod(act, env->GetMethodID(env->GetObjectClass(act), "onResume", "()V"));
}



