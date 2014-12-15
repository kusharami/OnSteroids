/*
 * Bitmap.h
 *
 *  Класс для загрузки картинок с помощью Java-среды.
 *
 *  Дата создания: 01.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <jni.h>

#include <android/bitmap.h>

// Класс для хранения картинки
class Bitmap
{
	public:
		// информация о размерах
		AndroidBitmapInfo info;

		uint32_t *pixels; // указатель на пиксели

		jobject bitmap; // объект типа Bitmap, созданный в Java-среде
		JNIEnv *env;	// Java-среда

		// уничтожить пиксели в деструкторе
		bool freePixelsOnDestroy;

		// инициировать картинку из буффера
		Bitmap(int width, int height, uint32_t *pixels, bool freePixelsOnDestroy)
		{
			this->info.width = width;
			this->info.height = height;
			this->info.stride = width;
			this->info.format = ANDROID_BITMAP_FORMAT_RGBA_8888;
			this->info.flags = 0;
			this->pixels = pixels;
			this->bitmap = NULL;
			this->env = NULL;
			this->freePixelsOnDestroy = freePixelsOnDestroy;
		};

		// загрузить картинку с помощью Java-среды
		Bitmap(jobject bitmap, JNIEnv *env, bool freePixelsOnDestroy);

		// получить пиксели картинки из Java-объекта bitmap
		int jLock();

		// разрешить изменение пикселей в Java-среде
		int jUnlock();

		virtual ~Bitmap();
};

// Класс для получения картинки
class BitmapFactory
{
	public:
		BitmapFactory(JNIEnv *env);

		// Функции получения картинки
		Bitmap* Decode(const void *data, int length);
		Bitmap* Decode(jbyteArray data);
		Bitmap* Copy(jobject bitmap);
		Bitmap* Copy(Bitmap *bitmap);
		Bitmap* Copy(jobject bitmap, int x, int y, int width, int height);
		Bitmap* Copy(Bitmap *bitmap, int x, int y, int width, int height);
		Bitmap* CreateEmpty(int width, int height, int config);
		Bitmap* CreateFrom(uint32_t *colors, int width, int height, int config);
		Bitmap* CreateFrom(uint32_t *colors, int stride, int width, int height, int config);
		Bitmap* Scaled(jobject bitmap, int newWidth, int newHeight, bool filter);
		Bitmap* Scaled(Bitmap *bitmap, int newWidth, int newHeight, bool filter);


	private:
		JNIEnv *env;
		jclass jBitmapFactory;	// Java-класс android.graphics.BitmapFactory
		jclass jBitmap;			// Java-класс android.graphics.Bitmap

		// Идентификаторы методов из Java-среды
		jmethodID decodeByteArray;

		//	public static Bitmap createBitmap (Bitmap src)
		jmethodID copyBitmap;

		//	public static Bitmap createBitmap (int width, int height, Bitmap.Config config)
		jmethodID createEmptyBitmap;

		//	public static Bitmap createBitmap (int[] colors, int width, int height, Bitmap.Config config)
		jmethodID createBitmapFrom;

		//	public static Bitmap createBitmap (Bitmap source, int x, int y, int width, int height)
		jmethodID copyBitmapArea;

		//	public static Bitmap createBitmap (int[] colors, int offset, int stride, int width, int height, Bitmap.Config config)
		jmethodID createBitmapFromEx;

		//	public static Bitmap createScaledBitmap (Bitmap src, int dstWidth, int dstHeight, boolean filter)
		jmethodID createScaledBitmap;
};

#endif /* BITMAP_H_ */
