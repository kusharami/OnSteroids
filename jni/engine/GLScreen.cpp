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
	// �������� �� ��������
	BitmapTexture* t = new BitmapTexture(bitmap, minFilter, magFilter, freeBitmap);
	// �������� � ������
	textures->Add()->object = t;
	return t;
}

Texture* GLScreen::AddCreatedTexture(Texture* texture)
{
	// �������� � ������
	textures->Add()->object = texture;
	return texture;
}

void GLScreen::PrepareTextures()
{
	ListItem* item = textures->root;
	while (item != NULL)
	{
		// ����������� �������� � ������
		((Texture*) item->object)->Prepare();
		item = item->next;
	}
}

void GLScreen::DisposeTextures()
{
	ListItem* item = textures->root;
	while (item != NULL)
	{
		// �������� ������ �� ��������
		((Texture*) item->object)->Dispose();
		item = item->next;
	}
}

void GLScreen::OnDrawFrame()
{
	// ������� ���������� �������, �� ������� ��� ��������� ���������� ����
	struct timespec newTime;
	clock_gettime(CLOCK_MONOTONIC, &newTime);
	Randomize(rmz + newTime.tv_nsec, rmw + newTime.tv_sec);

	uint64_t secDif = newTime.tv_sec - startTime.tv_sec;
	int32_t nsecDif = newTime.tv_nsec - startTime.tv_nsec;
	double latency = (secDif * 1000000000 + nsecDif) / 1000000.0;
	startTime = newTime;

	// ���������� ��������� ������
	Update(latency);

	// ���������
	RenderInit();
	Render();
	RenderDone();
}

void GLScreen::OnSurfaceChanged(int width, int height)
{
	// ��������� ��������
	SetSizes(width, height);

	// ��������� �� ���� �����
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	width = this->width;
	height = this->height;
	// ��������� ���������������� ��������
	glOrthof(0, width, height, 0, 1, -1);

	// �������� ����� ������ �����
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
	// ����� Java-������ ������������
	parent->javaEnvironment->CallVoidMethod(view, viewPause);
}

void GLScreen::RenderResume()
{
	// ����� Java-������ �����������
	parent->javaEnvironment->CallVoidMethod(view, viewPause);
}

GLScreen::GLScreen(GLActivity* parent) : drawWidth(0), drawHeight(0)
{
	// ������ ������ ��� �������� �������
	textures = new List((FreeObjectProc*) &TextureFree);
	// ������������� ���������
	SetParent(parent);
}

void GLScreen::SetParent(GLActivity* parent)
{
	this->parent = parent;
	if (parent != NULL)
	{
		JNIEnv* env = parent->javaEnvironment;
		jobject act = parent->javaActivity;
		// ��������� ������ Activity �� Java-�����
		jclass cls = env->GetObjectClass(act);
		// ����� ���� GLSurfaceView view � ���� ������
		jfieldID f = env->GetFieldID(cls, "view", "Landroid/opengl/GLSurfaceView;");
		// ��������� ������� �� ����� ����
		jobject view = this->view = env->GetObjectField(act, f);
		cls = env->GetObjectClass(view);
		// ��������� ������ GLSurfaceView.onPause
		this->viewPause = env->GetMethodID(cls, "onPause", "()V");
		// ��������� ������ GLSurfaceView.onResume
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




