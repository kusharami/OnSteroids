/*
 * GLScreen.cpp
 *
 *  Created on: 02.09.2013
 *      Author: Alicia
 */

#include "GLScreen.h"
#include "GLActivity.h"
#include "types.h"

BitmapTexture* GLScreen::AddBitmapTexture(Bitmap* bitmap, int minFilter, int magFilter, bool freeBitmap)
{
	BitmapTexture* t = new BitmapTexture(bitmap, minFilter, magFilter, freeBitmap);
	textures->Add()->object = t;
	return t;
}

Texture* GLScreen::AddCreatedTexture(Texture* texture)
{
	textures->Add()->object = texture;
	return texture;
}

void GLScreen::PrepareTextures()
{
	ListItem* item = textures->root;
	while (item != NULL)
	{
		((Texture*) item->object)->Prepare();
		item = item->next;
	}
}

void GLScreen::DisposeTextures()
{
	ListItem* item = textures->root;
	while (item != NULL)
	{
		((Texture*) item->object)->Dispose();
		item = item->next;
	}
}

void GLScreen::OnDrawFrame()
{
	struct timespec newTime;
	clock_gettime(CLOCK_MONOTONIC, &newTime);
	uint64_t secDif = 1000000000 * (uint64_t) (newTime.tv_sec - startTime.tv_sec);
	double latency = (secDif + (newTime.tv_nsec - startTime.tv_nsec)) / 1000000.0;
	startTime = newTime;

	Update(latency);

	RenderInit();
	Render();
	RenderDone();
}

void GLScreen::OnSurfaceChanged(int width, int height)
{
	setSizes(width, height);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	width = this->width;
	height = this->height;
	glOrthof(0, width, height, 0, 1, -1);

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
	GLActivity* p = (GLActivity*) parent;
	p->javaEnvironment->CallVoidMethod(view, viewPause);
}

void GLScreen::RenderResume()
{
	GLActivity* p = (GLActivity*) parent;
	p->javaEnvironment->CallVoidMethod(view, viewPause);
}

GLScreen::GLScreen(void* parent)
{
	SetParent(parent);

	Initialize();
}

void GLScreen::SetParent(void* parent)
{
	drawWidth = 0;
	drawHeight = 0;
	GLActivity* p = (GLActivity*) parent;
	textures = new List((FreeObjectProc*) &TextureFree);
	LOGI("GLScreen: textures list created");
	JNIEnv* env = p->javaEnvironment;
	jobject act = p->javaActivity;
	jclass cls = env->GetObjectClass(act);
	LOGI("GLScreen: activity class found");
	jfieldID f = env->GetFieldID(cls, "view", "Landroid/opengl/GLSurfaceView;");
	LOGI("GLScreen: field found");
	jobject view = this->view = env->GetObjectField(act, f);
	LOGI("GLScreen: view object found");
	cls = env->GetObjectClass(view);
	this->viewPause = env->GetMethodID(cls, "onPause", "()V");
	LOGI("GLScreen: onPause found");
	this->viewResume = env->GetMethodID(cls, "onResume", "()V");
	LOGI("GLScreen: onResume found");
}

void GLScreen::setSizes(int width, int height)
{
	this->viewWidth = width;
	this->viewHeight = height;
	this->width = getDrawWidth();
	this->height = getDrawHeight();
	this->scaleX = (float) width / (float) this->width;
	this->scaleY = (float) height / (float) this->height;
}




