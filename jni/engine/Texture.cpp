/*
 * Texture.cpp
 *
 *  Классы для хранения OpenGL текстур.
 *
 *  Дата создания: 01.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
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
		// создать одну текстуру
		glGenTextures(1, &tex_id);

		// активизировать
		glBindTexture(GL_TEXTURE_2D, tex_id);

		// подготовить пиксели
		PrepareTextureData();

		// задать фильтры
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		// конфигурировать
		TextureConfig();

		// деактивировать
		glBindTexture(GL_TEXTURE_2D, 0);

		prepared = true;
	}
}

void Texture::Bind()
{
	// активизировать
	glBindTexture(GL_TEXTURE_2D, tex_id);
}

void Texture::Dispose()
{
	if (prepared)
	{
		// деактивировать
		glBindTexture(GL_TEXTURE_2D, 0);
		// удалить из GL-памяти
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

	// привязать пиксели из картинки
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->info.width, bitmap->info.height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, bitmap->pixels);
}

//* DrawableTexture class *//

DrawableTexture::DrawableTexture(int w, int h, int minF, int magF): Texture(minF, magF)
{
	// задаём возможные размеры
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
	// пустая текстурка
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

void DrawableTexture::TextureConfig()
{
	// текстура с повторяющимися картинками
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

