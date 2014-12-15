/*
 * GLScreen.h
 *
 *   ласс дл€ вывода графики на экран с помощью OpenGL.
 *
 *  ƒата создани€: 02.09.2013
 *  ƒата изменени€: 16.09.2013
 *
 *  јвтор: „ерданцева јлександра
 */

#ifndef GLSCREEN_H_
#define GLSCREEN_H_

#include <jni.h>
#include <time.h>

#include "List.h"
#include "Texture.h"

class GLActivity;

class GLScreen
{
	public:
		// насто€ща€ ширина экрана
		int viewWidth;

		// насто€ща€ высота экрана
		int viewHeight;

		// пользовательска€ ширина
		int width;

		// пользовательска€ высота
		int height;

		// ћасштаб по X. –азница между начто€щей шириной экрана и пользовательской
		float scaleX;
		// ћасштаб по Y. –азница между начто€щей шириной экрана и пользовательской
		float scaleY;

		// список используемых текстур
		List* textures;

		// добавить текстуру из Bitmap
		BitmapTexture* AddBitmapTexture(Bitmap* bitmap, int minFilter, int magFilter, bool freeBitmap);

		// добавить уже готовую текстуру
		Texture* AddCreatedTexture(Texture* texture);

		// подготовить текстуры дл€ работы
		void PrepareTextures();

		// текстуры больше не нужны
		void DisposeTextures();

		// при создании новой поверхности подготовить текстуры к работе
		virtual void OnSurfaceCreated()	{	PrepareTextures();	};

		// задать размеры поверхности
		virtual void OnSurfaceChanged(int width, int height);

		// отрисовка экрана
		virtual void OnDrawFrame();

		// при паузе программы освободить текстуры
		virtual void OnActivityPause()	{	DisposeTextures();	};

		// действие, когда работа программы продолжаетс€ после паузы
		virtual void OnActivityResume() {};

		// приостановить отрисовку
		void RenderPause();

		// продолжить отрисовку
		void RenderResume();

		GLScreen(GLActivity* parent);

		virtual ~GLScreen()
		{
			delete textures;
		};

	protected:
		// Activity, владелец этого экрана
		GLActivity* parent;

		// GLSurfaceView из Java-среды
		jobject view;

		// Java-метод приостановки отрисовки
		jmethodID viewPause;

		// Java-метод продолжени€ отрисовки
		jmethodID viewResume;

		// пользовательска€ ширина по умолчанию
		int drawWidth;

		// пользовательска€ высота по умолчанию
		int drawHeight;

		// врем€ начала кадра
		struct timespec startTime;

		// установить владельца
		void SetParent(GLActivity* parent);

		// обновить состо€ние экрана
		// latency - количество времени в миллисекундах, которое прошло с момента предыдущего вызова этого метода
		virtual void Update(double latency) {};

		// перед отрисовкой
		virtual void RenderInit();

		// отрисовка
		virtual void Render() {};

		// после отрисовки
		virtual void RenderDone() {};

		// установка размеров поверхности
		void SetSizes(int width, int height);

		// получить пользовательскую ширину
		virtual int GetDrawWidth()
		{

			if (drawWidth <= 0)
				// если ширина не задана, возвращаем насто€щую ширину
				return viewWidth;
			else
				return drawWidth;
		}

		// получить пользовательскую высоту
		virtual int GetDrawHeight()
		{
			if (drawHeight <= 0)
				// если высота не задана, возвращаем насто€щую высоту
				return viewHeight;
			else
				return drawHeight;
		}
};

#endif /* GLSCREEN_H_ */
