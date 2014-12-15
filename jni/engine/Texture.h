/*
 * Texture.h
 *
 *  ������ ��� �������� OpenGL �������.
 *
 *  ���� ��������: 01.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GLES/gl.h>
#include "Bitmap.h"

class Texture
{
	public:
		// �������
		int width, height;
		// �������������
		GLuint tex_id;

		// GL-�������
		int minFilter;
		int magFilter;

		// ������ �� � �������������
		bool prepared;

		Texture(int minF, int magF) : minFilter(minF), magFilter(magF), width(0), height(0), tex_id(0), prepared(false) {};
		virtual ~Texture();

		// �����������
		void Prepare();

		// ������������
		void Bind();

		// ������ �� �����
		void Dispose();

	protected:
		// ������ �������
		virtual void PrepareTextureData() {};

		// ���������������
		virtual void TextureConfig() {};
};

// �������� �� Bitmap-��������
class BitmapTexture: public Texture
{
	public:
		// ��������
		Bitmap* bitmap;

		// ���������� �������� � ������������
		bool freeBitmapAfterUse;

		BitmapTexture(Bitmap *bitmap, int minF, int magF, bool freeBitmapAfterUse);
		virtual ~BitmapTexture();
	protected:
		// ������ �������
		virtual void PrepareTextureData();
};

// �������� � ������������� ���������
class DrawableTexture: public Texture
{
	public:
		DrawableTexture(int w, int h, int minF, int magF);

	protected:
		// ������ �������
		virtual void PrepareTextureData();

		// ���������������
		virtual void TextureConfig();
};

void TextureFree(Texture* texture);

#endif /* TEXTURE_H_ */
