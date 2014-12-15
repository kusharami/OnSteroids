/*
 * GLScreen.h
 *
 *  ����� ��� ������ ������� �� ����� � ������� OpenGL.
 *
 *  ���� ��������: 02.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
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
		// ��������� ������ ������
		int viewWidth;

		// ��������� ������ ������
		int viewHeight;

		// ���������������� ������
		int width;

		// ���������������� ������
		int height;

		// ������� �� X. ������� ����� ��������� ������� ������ � ����������������
		float scaleX;
		// ������� �� Y. ������� ����� ��������� ������� ������ � ����������������
		float scaleY;

		// ������ ������������ �������
		List* textures;

		// �������� �������� �� Bitmap
		BitmapTexture* AddBitmapTexture(Bitmap* bitmap, int minFilter, int magFilter, bool freeBitmap);

		// �������� ��� ������� ��������
		Texture* AddCreatedTexture(Texture* texture);

		// ����������� �������� ��� ������
		void PrepareTextures();

		// �������� ������ �� �����
		void DisposeTextures();

		// ��� �������� ����� ����������� ����������� �������� � ������
		virtual void OnSurfaceCreated()	{	PrepareTextures();	};

		// ������ ������� �����������
		virtual void OnSurfaceChanged(int width, int height);

		// ��������� ������
		virtual void OnDrawFrame();

		// ��� ����� ��������� ���������� ��������
		virtual void OnActivityPause()	{	DisposeTextures();	};

		// ��������, ����� ������ ��������� ������������ ����� �����
		virtual void OnActivityResume() {};

		// ������������� ���������
		void RenderPause();

		// ���������� ���������
		void RenderResume();

		GLScreen(GLActivity* parent);

		virtual ~GLScreen()
		{
			delete textures;
		};

	protected:
		// Activity, �������� ����� ������
		GLActivity* parent;

		// GLSurfaceView �� Java-�����
		jobject view;

		// Java-����� ������������ ���������
		jmethodID viewPause;

		// Java-����� ����������� ���������
		jmethodID viewResume;

		// ���������������� ������ �� ���������
		int drawWidth;

		// ���������������� ������ �� ���������
		int drawHeight;

		// ����� ������ �����
		struct timespec startTime;

		// ���������� ���������
		void SetParent(GLActivity* parent);

		// �������� ��������� ������
		// latency - ���������� ������� � �������������, ������� ������ � ������� ����������� ������ ����� ������
		virtual void Update(double latency) {};

		// ����� ����������
		virtual void RenderInit();

		// ���������
		virtual void Render() {};

		// ����� ���������
		virtual void RenderDone() {};

		// ��������� �������� �����������
		void SetSizes(int width, int height);

		// �������� ���������������� ������
		virtual int GetDrawWidth()
		{

			if (drawWidth <= 0)
				// ���� ������ �� ������, ���������� ��������� ������
				return viewWidth;
			else
				return drawWidth;
		}

		// �������� ���������������� ������
		virtual int GetDrawHeight()
		{
			if (drawHeight <= 0)
				// ���� ������ �� ������, ���������� ��������� ������
				return viewHeight;
			else
				return drawHeight;
		}
};

#endif /* GLSCREEN_H_ */
