/*
 * GLActivity.h
 *
 *  �++ ������ ������ Activity.
 *
 *  ���� ��������: 02.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef GLACTIVITY_H_
#define GLACTIVITY_H_

#include <jni.h>
#include <stdlib.h>

#include "Bitmap.h"
#include "AssetManager.h"
#include "GLScreen.h"
#include "InputListener.h"

// ������� �� ������ ��� ������ � ������� ����� OpenGL
// � ������ �������������� � �������������
class GLActivity: public GLScreen, public InputListener
{
	public:
		// Java-�����
		JNIEnv* javaEnvironment;

		// Activity ������ �� Java-�����
		jobject javaActivity;

		// ������ ��� �������� ��������
		BitmapFactory* bitmapFactory;

		// ������ � ������
		AssetManager* assets;

		virtual ~GLActivity();
		GLActivity(JNIEnv* env, jobject jActivity);

		// ��������� ���������
		void Terminate();

		// ������������� ������
		void Pause();

		// ���������� ������
		void Resume();

	protected:
		// ���� ���������� ���������
		bool terminating;
};

#endif /* GLACTIVITY_H_ */
