/*
 * GLScreen.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#include "Utils.h"
#include "GLScreen.h"
#include "GLActivity.h"

BitmapTexture* GLScreen::AddBitmapTexture(Bitmap* bitmap, int minFilter, int magFilter, bool freeBitmap)
{
	// текстура из картинки
	BitmapTexture* t = new BitmapTexture(bitmap, minFilter, magFilter, freeBitmap);
	// добавить в список
	textures->Add()->object = t;
	return t;
}

Texture* GLScreen::AddCreatedTexture(Texture* texture)
{
	// добавить в список
	textures->Add()->object = texture;
	return texture;
}

void GLScreen::PrepareTextures()
{
	ListItem* item = textures->root;
	while (item != NULL)
	{
		// подготовить текстуру к работе
		((Texture*) item->object)->Prepare();
		item = item->next;
	}
}

void GLScreen::DisposeTextures()
{
	ListItem* item = textures->root;
	while (item != NULL)
	{
		// текстура больше не работает
		((Texture*) item->object)->Dispose();
		item = item->next;
	}
}

void GLScreen::OnDrawFrame()
{
	// считаем количество времени, за которое был нарисован предыдущий кадр
	struct timespec newTime;
	clock_gettime(CLOCK_MONOTONIC, &newTime);
	Randomize(rmz + newTime.tv_nsec, rmw + newTime.tv_sec);

	uint64_t secDif = newTime.tv_sec - startTime.tv_sec;
	int32_t nsecDif = newTime.tv_nsec - startTime.tv_nsec;
	double latency = (secDif * 1000000000 + nsecDif) / 1000000.0;
	startTime = newTime;

	// обновление состояние экрана
	Update(latency);

	// отрисовка
	RenderInit();
	Render();
	RenderDone();
}

void GLScreen::OnSurfaceChanged(int width, int height)
{
	// установка размеров
	SetSizes(width, height);

	// видимость на весь экран
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	width = this->width;
	height = this->height;
	// установка пользовательских размеров
	glOrthof(0, width, height, 0, 1, -1);

	// получить время начала кадра
	clock_gettime(CLOCK_MONOTONIC, &startTime);
}

void GLScreen::RenderInit()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLScreen::RenderPause()
{
	// вызов Java-метода приостановки
	parent->javaEnvironment->CallVoidMethod(view, viewPause);
}

void GLScreen::RenderResume()
{
	// вызов Java-метода продолжения
	parent->javaEnvironment->CallVoidMethod(view, viewPause);
}

GLScreen::GLScreen(GLActivity* parent) : drawWidth(0), drawHeight(0)
{
	// создаём список для хранения текстур
	textures = new List((FreeObjectProc*) &TextureFree);
	// устанавливаем владельца
	SetParent(parent);
}

void GLScreen::SetParent(GLActivity* parent)
{
	this->parent = parent;
	if (parent != NULL)
	{
		JNIEnv* env = parent->javaEnvironment;
		jobject act = parent->javaActivity;
		// Получение класса Activity из Java-среды
		jclass cls = env->GetObjectClass(act);
		// поиск поля GLSurfaceView view в этом классе
		jfieldID f = env->GetFieldID(cls, "view", "Landroid/opengl/GLSurfaceView;");
		// получение объекта из этого поля
		jobject view = this->view = env->GetObjectField(act, f);
		cls = env->GetObjectClass(view);
		// получение метода GLSurfaceView.onPause
		this->viewPause = env->GetMethodID(cls, "onPause", "()V");
		// получение метода GLSurfaceView.onResume
		this->viewResume = env->GetMethodID(cls, "onResume", "()V");
	}
}

void GLScreen::SetSizes(int width, int height)
{
	this->viewWidth = width;
	this->viewHeight = height;

	this->width = GetDrawWidth();
	this->height = GetDrawHeight();

	this->scaleX = (float) width / (float) this->width;
	this->scaleY = (float) height / (float) this->height;
}




