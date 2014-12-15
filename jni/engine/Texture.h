/*
 * Texture.h
 *
 *  Классы для хранения OpenGL текстур.
 *
 *  Дата создания: 01.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GLES/gl.h>
#include "Bitmap.h"

class Texture
{
	public:
		// размеры
		int width, height;
		// идентификатор
		GLuint tex_id;

		// GL-фильтры
		int minFilter;
		int magFilter;

		// готова ли к использованию
		bool prepared;

		Texture(int minF, int magF) : minFilter(minF), magFilter(magF), width(0), height(0), tex_id(0), prepared(false) {};
		virtual ~Texture();

		// подготовить
		void Prepare();

		// активировать
		void Bind();

		// Больше не нужна
		void Dispose();

	protected:
		// задать пиксели
		virtual void PrepareTextureData() {};

		// конфигурировать
		virtual void TextureConfig() {};
};

// текстура из Bitmap-картинки
class BitmapTexture: public Texture
{
	public:
		// картинка
		Bitmap* bitmap;

		// уничтожить картинку в декструкторе
		bool freeBitmapAfterUse;

		BitmapTexture(Bitmap *bitmap, int minF, int magF, bool freeBitmapAfterUse);
		virtual ~BitmapTexture();
	protected:
		// задать пиксели
		virtual void PrepareTextureData();
};

// Текстура с произвольными пикселями
class DrawableTexture: public Texture
{
	public:
		DrawableTexture(int w, int h, int minF, int magF);

	protected:
		// задать пиксели
		virtual void PrepareTextureData();

		// конфигурировать
		virtual void TextureConfig();
};

void TextureFree(Texture* texture);

#endif /* TEXTURE_H_ */
