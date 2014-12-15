/*
 * Bitmap.cpp
 *
 *  Created on: 01.09.2013
 *      Author: Alicia
 */

#include <string.h>
#include "Bitmap.h"

BitmapFactory::BitmapFactory(JNIEnv *env)
{
	jclass jBitmapFactory = env->FindClass("android.graphics.BitmapFactory");
	jclass jBitmap = env->FindClass("android.graphics.Bitmap");
	decodeByteArray = env->GetStaticMethodID(jBitmapFactory, "decodeByteArray",
			"([BII)Landroid/graphics/Bitmap;");

//	public static Bitmap createBitmap (Bitmap src)
	copyBitmap = env->GetStaticMethodID(jBitmap, "createBitmap",
			"(Landroid/graphics/Bitmap;)Landroid/graphics/Bitmap;");

//	public static Bitmap createBitmap (int width, int height, Bitmap.Config config)
	createEmptyBitmap  = env->GetStaticMethodID(jBitmap, "createBitmap",
			"(IILandroid/graphics/Bitmap/Config;)Landroid/graphics/Bitmap;");

//	public static Bitmap createBitmap (int[] colors, int width, int height, Bitmap.Config config)
	createBitmapFrom = env->GetStaticMethodID(jBitmap, "createBitmap",
			"([IIILandroid/graphics/Bitmap.Config;)Landroid/graphics/Bitmap;");

//	public static Bitmap createBitmap (Bitmap source, int x, int y, int width, int height)
	copyBitmapArea = env->GetStaticMethodID(jBitmap, "createBitmap",
			"(Landroid/graphics/Bitmap;IIII)Landroid/graphics/Bitmap;");

//	public static Bitmap createBitmap (int[] colors, int offset, int stride, int width, int height, Bitmap.Config config)
	createBitmapFromEx = env->GetStaticMethodID(jBitmap, "createBitmap",
			"([IIIIILandroid/graphics/Bitmap.Config;)Landroid/graphics/Bitmap;");

//	public static Bitmap createScaledBitmap (Bitmap src, int dstWidth, int dstHeight, boolean filter)
	createScaledBitmap = env->GetStaticMethodID(jBitmap, "createScaledBitmap",
			"(Landroid/graphics/Bitmap;IIZ)Landroid/graphics/Bitmap;");

	this->env = env;
	this->jBitmapFactory = jBitmapFactory;
	this->jBitmap = jBitmap;
}

Bitmap* BitmapFactory::Decode(const void *data, int length)
{
	JNIEnv *env = this->env;
	jbyteArray a = env->NewByteArray(length);
	env->NewGlobalRef(a);
	jbyte *bytes = env->GetByteArrayElements(a, 0);
	memcpy(bytes, data, length);

	Bitmap* bitmap = Decode(a);

	env->ReleaseByteArrayElements(a, bytes, 0);
	env->DeleteGlobalRef(a);

	return bitmap;
}

Bitmap* BitmapFactory::Decode(jbyteArray data)
{
	JNIEnv *env = this->env;
	jobject b = env->CallStaticObjectMethod(jBitmapFactory, decodeByteArray, data, 0, env->GetArrayLength(data));
	env->NewGlobalRef(b);
	return new Bitmap(b, env, true);
}

Bitmap::Bitmap(jobject bitmap, JNIEnv *env, bool freePixelsOnDestroy)
{
	if (AndroidBitmap_getInfo(env, bitmap, &info) == 0 &&
		AndroidBitmap_lockPixels(env, bitmap, (void**) &pixels) == 0)
	{
		this->env = env;
		this->bitmap = bitmap;
		this->freePixelsOnDestroy = freePixelsOnDestroy;
	}
}

int Bitmap::jLock()
{
	jobject bitmap = this->bitmap;
	int error = AndroidBitmap_getInfo(env, bitmap, &info);
	if (error == 0)
		error = AndroidBitmap_lockPixels(env, bitmap, (void**) &pixels);
	return error;
}

int Bitmap::jUnlock()
{
	return AndroidBitmap_unlockPixels(env, bitmap);
}

Bitmap::~Bitmap()
{
	if (bitmap != NULL)
	{
		jobject bitmap = this->bitmap;
		if (AndroidBitmap_unlockPixels(env, bitmap) == 0)
		{
			if (freePixelsOnDestroy)
			{
				env->CallVoidMethod(bitmap, env->GetMethodID(env->GetObjectClass(bitmap), "recycle", "()V"));
				env->DeleteGlobalRef(bitmap);
			}
		}
	} else
	if (freePixelsOnDestroy)
		free(pixels);
}

Bitmap* BitmapFactory::Copy(jobject bitmap)
{
	JNIEnv *env = this->env;
	jobject b = env->CallStaticObjectMethod(jBitmap, copyBitmap, bitmap);
	env->NewGlobalRef(b);
	return new Bitmap(b, env, true);
}

Bitmap* BitmapFactory::Copy(Bitmap *bitmap)
{
	return Copy(bitmap->bitmap);
}

Bitmap* BitmapFactory::Copy(jobject bitmap, int x, int y, int width, int height)
{
	JNIEnv *env = this->env;
	jobject b = env->CallStaticObjectMethod(jBitmap, copyBitmapArea, bitmap, x, y, width, height);
	env->NewGlobalRef(b);
	return new Bitmap(b, env, true);
}

Bitmap* BitmapFactory::Copy(Bitmap *bitmap, int x, int y, int width, int height)
{
	return Copy(bitmap->bitmap, x, y, width, height);
}

Bitmap* BitmapFactory::CreateEmpty(int width, int height, int config)
{
	JNIEnv *env = this->env;
	jobject b = env->CallStaticObjectMethod(jBitmap, createEmptyBitmap, width, height, config);
	env->NewGlobalRef(b);
	return new Bitmap(b, env, true);
}

Bitmap* BitmapFactory::CreateFrom(uint32_t* colors, int width, int height, int config)
{
	JNIEnv *env = this->env;
	int length = width * height;
	jintArray a = env->NewIntArray(length);
	env->NewGlobalRef(a);
	jint *jColors = env->GetIntArrayElements(a, 0);
	memcpy(jColors, colors, length * 4);

	jobject b = env->CallStaticObjectMethod(jBitmap, createBitmapFrom, a, width, height, config);
	env->NewGlobalRef(b);

	env->ReleaseIntArrayElements(a, jColors, 0);
	env->DeleteGlobalRef(a);
	return new Bitmap(b, env, true);
}

Bitmap* BitmapFactory::CreateFrom(uint32_t* colors, int stride, int width, int height, int config)
{
	JNIEnv *env = this->env;
	int length = stride * height;
	jintArray a = env->NewIntArray(length);
	env->NewGlobalRef(a);
	jint *jColors = env->GetIntArrayElements(a, 0);
	memcpy(jColors, colors, length * 4);

	jobject b = env->CallStaticObjectMethod(jBitmap, createBitmapFromEx, a, 0, stride, width, height, config);
	env->NewGlobalRef(b);

	env->ReleaseIntArrayElements(a, jColors, 0);
	env->DeleteGlobalRef(a);
	return new Bitmap(b, env, true);
}

Bitmap* BitmapFactory::Scaled(jobject bitmap, int newWidth, int newHeight, bool filter)
{
	JNIEnv *env = this->env;
	jobject b = env->CallStaticObjectMethod(jBitmap, createScaledBitmap, bitmap, newWidth, newHeight, filter);
	env->NewGlobalRef(b);
	return new Bitmap(b, env, true);
}

Bitmap* BitmapFactory::Scaled(Bitmap *bitmap, int newWidth, int newHeight, bool filter)
{
	return Scaled(bitmap->bitmap, newWidth, newHeight, filter);
}


