/*
 * GLActivity.h
 *
 *  С++ версия класса Activity.
 *
 *  Дата создания: 02.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef GLACTIVITY_H_
#define GLACTIVITY_H_

#include <jni.h>
#include <stdlib.h>

#include "Bitmap.h"
#include "AssetManager.h"
#include "GLScreen.h"
#include "InputListener.h"

// основан на классе для работы с экраном через OpenGL
// и классе взаимодействия с пользователем
class GLActivity: public GLScreen, public InputListener
{
	public:
		// Java-среда
		JNIEnv* javaEnvironment;

		// Activity объект из Java-среды
		jobject javaActivity;

		// объект для загрузки картинок
		BitmapFactory* bitmapFactory;

		// Доступ к файлам
		AssetManager* assets;

		virtual ~GLActivity();
		GLActivity(JNIEnv* env, jobject jActivity);

		// завершить программу
		void Terminate();

		// приостановить работу
		void Pause();

		// продолжить работу
		void Resume();

	protected:
		// флаг завершения программы
		bool terminating;
};

#endif /* GLACTIVITY_H_ */
