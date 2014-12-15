/*
 * Texture.cpp
 *
 *  ������ ��� �������� OpenGL �������.
 *
 *  ���� ��������: 01.09.2013
 *  ���� ���������: 16.09.2013
 *
 *  �����: ���������� ����������
 */

#include "Texture.h"

void TextureFree(Texture* texture)
{
	delete texture;
}

//* Texture class *//

void Texture::Prepare()
{
	if (!prepared)
	{
		// ������� ���� ��������
		glGenTextures(1, &tex_id);

		// ��������������
		glBindTexture(GL_TEXTURE_2D, tex_id);

		// ����������� �������
		PrepareTextureData();

		// ������ �������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		// ���������������
		TextureConfig();

		// ��������������
		glBindTexture(GL_TEXTURE_2D, 0);

		prepared = true;
	}
}

void Texture::Bind()
{
	// ��������������
	glBindTexture(GL_TEXTURE_2D, tex_id);
}

void Texture::Dispose()
{
	if (prepared)
	{
		// ��������������
		glBindTexture(GL_TEXTURE_2D, 0);
		// ������� �� GL-������
		glDeleteTextures(1, &tex_id);
		prepared = false;
	}
}

Texture::~Texture()
{
	Dispose();
}

//* BitmapTexture class *//

BitmapTexture::BitmapTexture(Bitmap *bitmap, int minF, int magF, bool freeBitmapAfterUse) : Texture(minF, magF)
{
	this->width = bitmap->info.width;
	this->height = bitmap->info.height;
	this->bitmap = bitmap;
	this->freeBitmapAfterUse = freeBitmapAfterUse;
}

BitmapTexture::~BitmapTexture()
{
	if (freeBitmapAfterUse)
		delete bitmap;
}

void BitmapTexture::PrepareTextureData()
{
	Bitmap *bitmap = this->bitmap;

	// ��������� ������� �� ��������
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->info.width, bitmap->info.height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, bitmap->pixels);
}

//* DrawableTexture class *//

DrawableTexture::DrawableTexture(int w, int h, int minF, int magF): Texture(minF, magF)
{
	// ����� ��������� �������
	int i = 16;

	while (w > i && i < 2048)
		i <<= 1;
	w = i;

	i = 16;
	while (h > i && i < 2048)
		i <<= 1;

	h = i;
	width = w;
	height = h;
}

void DrawableTexture::PrepareTextureData()
{
	// ������ ���������
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

void DrawableTexture::TextureConfig()
{
	// �������� � �������������� ����������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

